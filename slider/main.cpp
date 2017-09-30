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

//#include <wiringPi.h>

int main (int argc, char *argv[])
{
    HttpServer server(8080, 4);
    
    server.resource["^/string$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        stringstream ss;
        ss << request->content.rdbuf();
        string content=ss.str();
        
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
    
    server.resource["^/json$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        try {
            ptree pt;
            read_json(request->content, pt);

            string name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");
string hole="hola";
cout<<name<<endl;
            response << "HTTP/1.1 200 OK\r\nContent-Length: " << hole.length() << "\r\n\r\n" << hole;
        }
        catch(exception& e) {
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };
    
    server.resource["^/info$"]["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for(auto& header: request->header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }
        
        content_stream.seekp(0, ios::end);
        
        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
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
    
    server_thread.join();
//	wiringPiSetup();
	std::cout<<"Init program"<<std::endl;  	
	context* cont = get_context();
	context_construct(cont);
	cart car;

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
						std::cout<<"Impossible values"<<std::endl;
						cont->status = STATUS_FINISHED;
					}
				}
				break;
			case STATUS_IDLE:
				cont->status = STATUS_RUNNING;
				car.print_config();
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
						break;
					default:
						break;
				}	
				break;
				//program finished
			case STATUS_FINISHED:

				return 0;
				break;

		}
		if(car.m_prog == PROG_FINISH)
		{
			return 0;

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//car.print_status();
	}
	return 0;

}


