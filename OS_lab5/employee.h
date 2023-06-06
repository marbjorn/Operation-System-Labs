#include <ostream>
struct employee {
    int num = 0;
    char name[10];
    double hours = 0;
    void print(std::ostream &out){
        out << "ID: " << num
        << "\tName: " << name
        << "\tHours: " << hours << std::endl;
    }
};

int empCmp(const void* p1, const void* p2){
    return reinterpret_cast<employee*>(const_cast<void*>(p1))->num - reinterpret_cast<employee*>(const_cast<void*>(p2))->num;
}