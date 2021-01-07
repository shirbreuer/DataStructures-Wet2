CXX = g++
EXEC = DS
OBJS = main.o class_node.o course_node.o data_structure.o library.o #avl_node.o avl_tree.o
HFLS = avl_node.h avl_tree.h class_node.h course_node.h data_structure.h  #library.h
CPPFLS = class_node.cpp course_node.cpp data_structure.cpp #library.cpp main.cpp
HPPFLS = avl_node.hpp avl_tree.hpp two_way_list.hpp two_way_list_node.hpp
MISCFLS = design.pdf submissions.txt
COMP_FLAG = -std=c++11  -Wall -g #-Werror -pedantic-errors  -g -fPIC
DEBUG_FLAG = -DNDEBUG

$(EXEC): $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@

main.o: main.cpp avl_tree.h avl_tree.hpp avl_node.h avl_node.hpp
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

class_node.o: class_node.cpp class_node.h avl_node.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

course_node.o: course_node.cpp course_node.h avl_node.h avl_node.hpp class_node.h two_way_list.hpp two_way_list_node.hpp avl_tree.h library.h #data_structure.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

data_structure.o: data_structure.cpp data_structure.h avl_tree.h avl_tree.hpp avl_node.h class_node.h course_node.h two_way_list.hpp two_way_list_node.hpp library.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

library.o: library.cpp library.h data_structure.h avl_tree.h avl_tree.hpp avl_node.h class_node.h course_node.h two_way_list.hpp two_way_list_node.hpp
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# libgraph.a: $(OBJS)
# 	ar -rs $@ $^

test:
	make 

tar:  $(HFLS) $(CPPFLS) $(HPPFLS) $(MISCFLS)
	zip Wet1.zip $(EXEC) $(HFLS) $(CPPFLS) $(HPPFLS) $(MISCFLS)

final: $(HFLS) $(CPPFLS) test_in.txt test_out.txt Makefile design.pdf libgraph.a graph.i
	rm -f gcalc.zip
	make clean
	make
	make swig
	make libgraph.a
	make tar
	/home/mtm/public/1920b/project/finalCheck gcalc.zip

clean:
	rm -f $(OBJS) $(EXEC)

val:
	valgrind --leak-check=full --track-origins=yes ./DS < in1.txt > out1.txt

valv:
	valgrind --leak-check=full --track-origins=yes ./DS < in1.txt > out1.txt

call:
	valgrind --tool=callgrind ./gcalc test.in test.out

run:
	./gcalc test.in test.out

swig: graph.i
	make libgraph.a
	swig -python -o graph_wrap.c  graph.i
	g++ -std=c++11 -DNDEBUG --pedantic-errors -Wall -Werror -I/usr/local/include/python3.6m -fPIC -o _graph.so -shared graph_wrap.c libgraph.a 

valv2:
	make test
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in2.txt > all_tests/myout2.txt


test2:
	./DS < all_tests/in2.txt > all_tests/myout2.txt
	diff all_tests/myout2.txt all_tests/out2.txt

val2:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in2.txt > all_tests/myout2.txt

test3:
	./DS < all_tests/in3.txt > all_tests/myout3.txt
	diff all_tests/myout3.txt all_tests/out3.txt

val3:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in3.txt > all_tests/myout3.txt


test4:
	./DS < all_tests/in4.txt > all_tests/myout4.txt
	diff all_tests/myout4.txt all_tests/out4.txt

val4:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in4.txt > all_tests/myout4.txt



test5:
	./DS < all_tests/in5.txt > all_tests/myout5.txt
	diff all_tests/myout5.txt all_tests/out5.txt

val5:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in5.txt > all_tests/myout5.txt


test6:
	./DS < all_tests/in6.txt > all_tests/myout6.txt
	diff all_tests/myout6.txt all_tests/out6.txt

val6:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in6.txt > all_tests/myout6.txt

test7:
	./DS < all_tests/stressin7.txt > all_tests/myout7.txt
	diff all_tests/myout7.txt all_tests/stressout7.txt

val7:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/stressin7.txt > all_tests/myout7.txt


test8:
	./DS < all_tests/stressin8.txt > all_tests/myout8.txt
	diff all_tests/myout8.txt all_tests/stressout8.txt

val8:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/stressin8.txt > all_tests/myout8.txt


test9:
	./DS < all_tests/stressin9.txt > all_tests/myout9.txt
	diff all_tests/myout9.txt all_tests/stressout9.txt

val9:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/stressin9.txt > all_tests/myout9.txt


test10:
	./DS < all_tests/stressin10.txt > all_tests/myout10.txt
	diff all_tests/myout10.txt all_tests/stressout10.txt

val10:
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/stressin10.txt > all_tests/myout10.txt

test11:
	./DS < all_tests/simplein11.txt > all_tests/myout11.txt
	diff all_tests/myout11.txt all_tests/simpleout11.txt

test12:
	./DS < all_tests/simplein12.txt > all_tests/myout12.txt
	diff all_tests/myout12.txt all_tests/simpleout12.txt

test13:
	./DS < all_tests/in13.txt > all_tests/myout13.txt
	#diff all_tests/myout12.txt all_tests/simpleout12.txt

val13:
	./DS < all_tests/in13.txt > all_tests/myout13.txt
	valgrind --leak-check=full --track-origins=yes ./DS < all_tests/in13.txt > all_tests/myout13.txt

try:
	make test
	make test2
	make test3
	make test4
	make test5
	make test6
	make test7
	make test8
	make test9
	make test10
	make test11
	make test12

try_val:
	make test
	make val2
	make val3
	make val4
	make val5
	make val6
	make val7
	make val8
	make val9
	make val10



