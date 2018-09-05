from http.server import BaseHTTPRequestHandler, HTTPServer
import time

# define ip:port
hostName = "10.100.120.115"
hostPort = 8988


class MyServer(BaseHTTPRequestHandler):

    # GET is for clients geting the predi
    def do_GET(self):
        self.send_response(200)
        self.wfile.write(bytes("<p>You accessed path: %s</p>" % self.path, "utf-8"))

    # POST is for submitting data.
    def do_POST(self):
        print("incomming http: ", self.path)

        # <--- Gets the size of data
        content_length = int(self.headers['Content-Length'])
        # <--- Gets the data itself
        post_data = self.rfile.read(content_length)
        print(post_data)
        self.send_response(200)
        # client.close()
        # import pdb; pdb.set_trace()


myServer = HTTPServer((hostName, hostPort), MyServer)
print(time.asctime(), "Server Starts - %s:%s" % (hostName, hostPort))

try:
    myServer.serve_forever()
except KeyboardInterrupt:
    pass

myServer.server_close()
print(time.asctime(), "Server Stops - %s:%s" % (hostName, hostPort))
