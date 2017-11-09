#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "context.h"
#include "cart.h"
#include "status.h"
#include "parse.h"

#include "server_http.hpp"

//Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Added for the default_resource example
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

#include <wiringPi.h>

cart* pcar;
context* pcon;

int main (int argc, char *argv[])
{
	context* cont = get_context();
	context_construct(cont);
	cart car;
	pcon=cont;
	pcar = &car;

	HttpServer server(8080, 4);

	server.resource["^/set$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
		string error_message;
		try {
			ptree pt;
			read_json(request->content, pt);
			string obt	=pt.get<string>("obturacion");
			string dur	=pt.get<string>("duracion");
			string fot	=pt.get<string>("fotos");
			string dis	=pt.get<string>("distancia");
			if( !pcar->set_program(
						std::stoi(obt),
						std::stoi(dur),
						std::stoi(fot),
						std::stoi(dis)) )
			{
				error_message = "Impossible values";
			}else
			{
				pcon->status=STATUS_IDLE;
				pcon->mode=MODE_PROGRAM;
				response << "HTTP/1.1 200 OK\r\n";
			}
		}
		catch(exception& e) {
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << error_message.length() << "\r\n\r\n" << error_message;
		}
	};

	server.resource["^/control$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
		string error_message;
		try {
			ptree pt;
			read_json(request->content, pt);

			string control=pt.get<string>("control");
			if ( control == "start")
			{
				if( pcon->status == STATUS_IDLE)
				{
					pcon->status = STATUS_RUNNING;
				}
				else if ( pcon->status == STATUS_INIT )
				{
					error_message = "Por favor, configura el programa antes de iniciar";
				}
				else if (pcon->status == STATUS_RUNNING )
				{
					error_message = "Programa ya en ejecucion";
				}

			}
			else if ( control == "stop" )
			{
				if( pcon->status == STATUS_IDLE)
				{
					error_message = "Programa no iniciado";
				}
				else if ( pcon->status == STATUS_INIT )
				{
					error_message = "Programa no iniciado ni configurado";
				}
				else if (pcon->status == STATUS_RUNNING )
				{
					pcon->status = STATUS_FINISHED;
				}	
			}
			else if(control == "open")
			{
				pcon->status = STATUS_RUNNING;
				pcon->mode = MODE_MANUAL;
				string timer=pt.get<string>("tiempo");
				pcon->timer = stoi(timer); 
				std::cout<<"timer"<<std::endl;
			}
			else if (control == "close")
			{
				pcon->status = STATUS_FINISHED;
				std::cout<<"stop"<<std::endl;
			}
			else
			{
				error_message = "Bad control command";
			}
			if (error_message.length() )
			{
				response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << error_message.length() << "\r\n\r\n" << error_message;
			}
			else
			{
				response << "HTTP/1.1 200 OK\r\n";
			}
		}

		catch(exception& e) {
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << error_message.length() << "\r\n\r\n" << error_message;
		}
	};
	server.resource["^/shoot$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
		string error_message;
		try {
			ptree pt;
			read_json(request->content, pt);

			string action=pt.get<string>("action");
			string tiempo=pt.get<string>("time");
			//long int time=stoi(tiempo);

			response << "HTTP/1.1 200 OK\r\n";
		}
		catch(exception& e) {
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << error_message.length() << "\r\n\r\n" << error_message;
		}
	};
	server.resource["^/move$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
		string error_message;
		try {
			ptree pt;
			read_json(request->content, pt);

			string position	=pt.get<string>("move");

			if(position=="init")
			{
				pcon->mode=MODE_TO_START;
				pcon->status=STATUS_RUNNING;
			}
			else if(position=="end")
			{
				pcon->mode=MODE_TO_END;
				pcon->status=STATUS_RUNNING;
			}
			else if(position=="stop")
			{
				pcon->status=STATUS_FINISHED;
			}

			response << "HTTP/1.1 200 OK\r\n";
		}
		catch(exception& e) {
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << error_message.length() << "\r\n\r\n" << error_message;
		}
	};

	server.default_resource["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
		boost::filesystem::path web_root_path("../web");
		if(!boost::filesystem::exists(web_root_path))
			cerr << "Could not find web root." << endl;
		else {
			auto path=web_root_path;
			path+=request->path;
			if(boost::filesystem::exists(path)) {
				if(boost::filesystem::canonical(web_root_path)<=boost::filesystem::canonical(path)) {
					if(boost::filesystem::is_directory(path))
						path+="/index.html";
					if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)) {
						ifstream ifs;
						ifs.open(path.string(), ifstream::in | ios::binary);

						if(ifs) {
							ifs.seekg(0, ios::end);
							size_t length=ifs.tellg();

							ifs.seekg(0, ios::beg);

							response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";

							//read and send 128 KB at a time
							size_t buffer_size=131072;
							vector<char> buffer;
							buffer.reserve(buffer_size);
							size_t read_length;
							try {
								while((read_length=ifs.read(&buffer[0], buffer_size).gcount())>0) {
									response.write(&buffer[0], read_length);
									response.flush();
								}
							}
							catch(const exception &e) {
								cerr << "Connection interrupted, closing file" << endl;
							}

							ifs.close();
							return;
						}
					}
				}
			}
		}
		string content="Could not open path "+request->path;
		response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
	};

	thread server_thread([&server](){
			//Start server
			server.start();
			});

	wiringPiSetup();

	for(;;)
	{
		switch(cont->status)
		{
			//read parameters and decide mode of operation
			case STATUS_INIT:
				parse(cont, argc, argv);
				if(cont->mode==MODE_PROGRAM)
				{
					if( !car.set_program(
								std::stoi(std::string(argv[2])),
								std::stoi(std::string(argv[3])),
								std::stoi(std::string(argv[4])),
								std::stoi(std::string(argv[5])) ) )
					{
						cont->status = STATUS_FINISHED;
					}
				}
				break;
			case STATUS_IDLE:
				//wait for init signal
				break;
				//running mode
			case STATUS_RUNNING:
				switch(cont->mode)
				{
					case MODE_PROGRAM:
						car.program();
						break;
					case	MODE_TO_END:
						car.move(DIR::END, cont->speed);
						break;
					case	MODE_TO_START:
						car.move(DIR::START, cont->speed);
						break;
					case	MODE_MANUAL:
						car.take(cont->timer);
						break;
					default:
						break;
				}	
				break;
				//program finished
			case STATUS_FINISHED:
				car.m_prog = PROG_FINISH;
				cont->status = STATUS_IDLE;
			default:
				break;

		}
		if(car.m_prog == PROG_FINISH)
		{
			context_construct(cont);
			car=cart();
			pcar = &car;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	server_thread.join();

	return 0;

}


