class DisjointSet {

private:
	std::vector<int> jobs;
	int count;

public:
	DisjointSet(int n);
	bool sameComponent(unsigned a, unsigned b);
	int find(int a);
	void merge(int a, int b);
	bool stronglyConnected();
};