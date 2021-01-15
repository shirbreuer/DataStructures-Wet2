CXX = g++
EXEC = a.out
OBJS = main.o class_node.o course_node.o data_structure.o library.o #avl_node.o avl_tree.o
HFLS = avl_node.h avl_tree.h class_node.h course_node.h data_structure.h  hash_table.h
CPPFLS = class_node.cpp course_node.cpp data_structure.cpp library.cpp #main.cpp
HPPFLS = avl_node.hpp avl_tree.hpp two_way_list.hpp two_way_list_node.hpp hash_table.hpp
MISCFLS = design.pdf submissions.txt
COMP_FLAG = -std=c++11  -Wall -g #-Werror -pedantic-errors  -g -fPIC
DEBUG_FLAG = -DNDEBUG

#    .---------- constant part!
#    vvvv vvvv-- the code from above
RED=\033[1;34m
NC=\033[0m # No Color


# $(EXEC): $(OBJS)
# 	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@

# main.o: main.cpp library.h
# 	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# class_node.o: class_node.cpp class_node.h avl_node.h
# 	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# course_node.o: course_node.cpp course_node.h avl_node.h avl_node.hpp class_node.h two_way_list.hpp two_way_list_node.hpp avl_tree.h library.h #data_structure.h
# 	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# data_structure.o: data_structure.cpp data_structure.h avl_tree.h avl_tree.hpp avl_node.h class_node.h course_node.h two_way_list.hpp two_way_list_node.hpp library.h
# 	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# library.o: library.cpp library.h data_structure.h avl_tree.h avl_tree.hpp avl_node.h class_node.h course_node.h two_way_list.hpp two_way_list_node.hpp
# 	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

# libgraph.a: $(OBJS)
# 	ar -rs $@ $^

compile:
	g++ -g -std=c++11 -Wall -DNDEBUG *.cpp

main:
	g++ -g -std=c++11 -Wall -DNDEBUG main.cpp

tar:  $(HFLS) $(CPPFLS) $(HPPFLS) $(MISCFLS)
	zip Wet2.zip $(HFLS) $(CPPFLS) $(HPPFLS) $(MISCFLS)

# final: $(HFLS) $(CPPFLS) test_in.txt test_out.txt Makefile design.pdf libgraph.a graph.i
# 	rm -f gcalc.zip
# 	make clean
# 	make
# 	make swig
# 	make libgraph.a
# 	make tar
# 	/home/mtm/public/1920b/project/finalCheck gcalc.zip

clean:
	rm -f $(OBJS) $(EXEC)

val_int:
	valgrind --leak-check=full --track-origins=yes ./a.out

#to run, type make val v=(number)
val:
	valgrind --leak-check=full --track-origins=yes ./a.out < segel_tests/in$(v).txt > segel_tests/myout$(v).txt

valv:
	valgrind --leak-check=full --track-origins=yes -v ./a.out < segel_tests/in$(v).txt > segel_tests/myout$(v).txt

call:
	valgrind --tool=callgrind ./gcalc test.in test.out


test:
	./a.out < segel_tests/in$(v).txt > segel_tests/myout$(v).txt
	diff segel_tests/out$(v).txt segel_tests/myout$(v).txt > segel_tests/mydiff$(v).txt

try:
	number=1 ; while [[ $$number -le 10 ]] ; do \
	echo -e "${RED}test number $$number: ${NC}" ; \
	make test v=$$number 2>&1 >/dev/null | grep "ERROR"; \
	((number = number + 1)) ; \
	done


try_val:
	number=1 ; while [[ $$number -le 10 ]] ; do \
	echo -e "${RED}test number $$number: ${NC}" ; \
	make val v=$$number 2>&1 >/dev/null | grep "ERROR SUMMARY"; \
	((number = number + 1)) ; \
	done

try_valv:
	number=1 ; while [[ $$number -le 10 ]] ; do \
	echo -e "${RED}test number $$number: ${NC}" ; \
	make valv v=$$number 2>&1 >/dev/null | grep "ERROR SUMMARY"; \
	((number = number + 1)) ; \
	done

