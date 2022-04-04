SOURCES=HttpCachingProxy.cpp MyData.cpp UniqueID.cpp Request.cpp ServerSocket.cpp ClientSocket.cpp GETServer.cpp \
CONNECTServer.cpp POSTServer.cpp Response.cpp Server.cpp Cache.cpp
LIBS=-lpthread
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-Wall -Werror -pedantic -std=c++14

proxy: $(OBJS)
	g++ $(CPPFLAGS) -o httpCachingProxy $(OBJS) $(LIBS)
%.o: %.cpp HttpCachingProxy.h
	g++ $(CPPFLAGS) -c $<

clean:
	rm -f httpCachingProxy *.o
