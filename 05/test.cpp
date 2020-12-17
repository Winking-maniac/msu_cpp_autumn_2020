#include "serializer.h"
#include <sstream>
#include <assert.h>

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
    
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

struct InvalidData
{
    uint64_t a;
    bool b;
    uint64_t c;
    float d;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, d);
    }
    
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c, d);
    }
};


int main() {
    std::stringstream stream;
    Serializer serializer(stream);
    
    // Save test
    Data x { 1, true, 2 };
    assert(serializer.save(x) == Error::NoError);
    assert(stream.str() == "1 true 2 ");
    
    // Load test
    Data y { 0, false, 0 };    
    Deserializer deserializer(stream);
    assert(deserializer.load(y) == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    
    // Invalid save test
    InvalidData z {1, true, 3, 5.0};
    assert(serializer.save(z) == Error::CorruptedArchive);
    
    // Invalid load test
    stream.str("1 true 3 5.0");
    assert(serializer.save(z) == Error::CorruptedArchive);
    
    std::cout << std::endl << "All tests passed" << std::endl;
}
