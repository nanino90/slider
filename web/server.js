function init_server()	{
		HttpServer server = HttpServer.create(new InetSocketAddress(8080), 0);
		server.createContext("/display", MyHandler());
		server.setExecutor(null); // creates a default executor
		server.start();
	}
}
function MyHandler implements HttpHandler {
		public void handle(HttpExchange t) throws IOException {
			String response = "hello world";
			t.sendResponseHeaders(200, response.length());
			System.out.println(response);
			OutputStream os = t.getResponseBody();
			os.write(response.getBytes());
			os.close();
		}
	}
}

