CXXFLAGS = -fPIC -std=c++11 -Wall -lpthread

LIB_NAME = libparallel_scheduler.so
LIB_SRC = parallel_scheduler.cpp
DEMO_NAME = demo
DEMO_SRC = main.cpp

all: $(LIB_NAME) $(DEMO_NAME)

$(LIB_NAME): $(LIB_SRC)
	g++ $(CXXFLAGS) -shared -o $(LIB_NAME) $(LIB_SRC)


$(DEMO_NAME): $(DEMO_SRC) $(LIB_NAME)
	g++ $(CXXFLAGS) -o $(DEMO_NAME) $(DEMO_SRC) -L. -lparallel_scheduler

