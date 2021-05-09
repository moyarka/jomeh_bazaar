CC=g++
STD=-std=c++11 
# -Wall -pedantic
CF=$(STD)
CC_STD=-std=c++11
BUILD_DIR=build
TEMPLATE_DIR=.template

all: $(BUILD_DIR) myserver.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) $(CF) -c utils/template_parser.cpp -o $(BUILD_DIR)/template_parser.o

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/handlers.o: examples/handlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c examples/handlers.cpp -o $(BUILD_DIR)/handlers.o

$(BUILD_DIR)/my_server.o: examples/my_server.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c examples/my_server.cpp -o $(BUILD_DIR)/my_server.o

$(BUILD_DIR)/main.o: examples/main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CC_OPT) -c examples/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/comment_labeller.o : market/comment_labeller.cpp market/comment_labeller.hpp
	$(CC) $(C_STD) -c market/comment_labeller.cpp -o $(BUILD_DIR)/comment_labeller.o

$(BUILD_DIR)/utilities.a : $(BUILD_DIR)/misc_funcs.o $(BUILD_DIR)/sort_methods.o
	ar rvs $(BUILD_DIR)/utilities.a $(BUILD_DIR)/misc_funcs.o $(BUILD_DIR)/sort_methods.o

$(BUILD_DIR)/market.a : $(BUILD_DIR)/market.o $(BUILD_DIR)/find_and_check.o $(BUILD_DIR)/processing.o $(BUILD_DIR)/print.o
	ar rvs $(BUILD_DIR)/market.a $(BUILD_DIR)/market.o $(BUILD_DIR)/find_and_check.o $(BUILD_DIR)/processing.o $(BUILD_DIR)/print.o

$(BUILD_DIR)/offer.o : market/offer.cpp market/offer.hpp
	$(CC) $(CC_STD) -c market/offer.cpp -o $(BUILD_DIR)/offer.o

$(BUILD_DIR)/product.o : market/product.cpp market/product.hpp
	$(CC) $(CC_STD) -c market/product.cpp -o $(BUILD_DIR)/product.o

$(BUILD_DIR)/user.o : market/user.cpp market/user.hpp
	$(CC) $(CC_STD) -c market/user.cpp -o $(BUILD_DIR)/user.o

$(BUILD_DIR)/market.o : market/market.cpp market/market.hpp market/sort_methods.hpp
	$(CC) $(CC_STD) -c market/market.cpp -o $(BUILD_DIR)/market.o

$(BUILD_DIR)/find_and_check.o : market/find_and_check.cpp market/market.hpp market/exceptions.hpp
	$(CC) $(CC_STD) -c market/find_and_check.cpp -o $(BUILD_DIR)/find_and_check.o

$(BUILD_DIR)/misc_funcs.o : market/misc_funcs.cpp market/misc_funcs.hpp
	$(CC) $(CC_STD) -c market/misc_funcs.cpp -o $(BUILD_DIR)/misc_funcs.o

$(BUILD_DIR)/processing.o : market/processing.cpp market/market.hpp
	$(CC) $(CC_STD) -c market/processing.cpp -o $(BUILD_DIR)/processing.o

$(BUILD_DIR)/print.o : market/print.cpp market/market.hpp market/sort_methods.hpp
	$(CC) $(CC_STD) -c market/print.cpp -o $(BUILD_DIR)/print.o

$(BUILD_DIR)/sort_methods.o : market/sort_methods.cpp market/sort_methods.hpp
	$(CC) $(CC_STD) -c market/sort_methods.cpp -o $(BUILD_DIR)/sort_methods.o

myserver.out: $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o $(BUILD_DIR)/market.a $(BUILD_DIR)/utilities.a $(BUILD_DIR)/offer.o $(BUILD_DIR)/user.o $(BUILD_DIR)/product.o $(BUILD_DIR)/comment_labeller.o
	$(CC) $(CC_STD) $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o $(BUILD_DIR)/market.a $(BUILD_DIR)/utilities.a $(BUILD_DIR)/offer.o $(BUILD_DIR)/user.o $(BUILD_DIR)/product.o $(BUILD_DIR)/comment_labeller.o -o myserver.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
