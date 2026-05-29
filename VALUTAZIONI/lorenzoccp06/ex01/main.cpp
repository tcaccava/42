# include "Serializer.hpp"

int main(void) {
	Data * d1 = new Data;
	Data * d2 = NULL;

	d1->nbr = 42;
	d1->str = "Hello World!";
	
	std::cout << "Original Data: " << *d1 << " addr: " << d1 << std::endl;

	uintptr_t raw = Serializer::serialize(d1);
	
	std::cout << "Serialized Data: " << raw << std::endl;

	d2 = Serializer::deserialize(raw);
	std::cout << "Deserialized Data: " << *d2 << " addr: " << d2 << std::endl;

	delete d1;
}