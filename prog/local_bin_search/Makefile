default: test.cpp local_bin_search.hpp simulated_annealing.hpp local_order_in_bins.hpp lower_bound.hpp
	g++ test.cpp -o test -std=c++11 -Wall -O2

test2: test2.cpp local_bin_search.hpp simulated_annealing.hpp local_order_in_bins.hpp lower_bound.hpp
	g++ test2.cpp -o test2 -std=c++11 -Wall -O2

bound: bound.cpp local_bin_search.hpp simulated_annealing.hpp local_order_in_bins.hpp lower_bound.hpp
	g++ bound.cpp -o bound -std=c++11 -Wall -O2

sa: sa.cpp local_bin_search.hpp simulated_annealing.hpp local_order_in_bins.hpp lower_bound.hpp
	g++ sa.cpp -o sa -std=c++11 -Wall -O2


test: default
	./test
