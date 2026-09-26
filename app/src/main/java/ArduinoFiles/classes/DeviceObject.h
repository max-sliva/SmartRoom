#ifndef DeviceObject_h
#define DeviceObject_h
/**
    Abstract class that represents a device connecting to SmartHouse system
*/
class DeviceObject {
public:
    const unsigned int _id = 0;

    DeviceObject() {}

    DeviceObject(const unsigned int id) : _id(id) {}

    unsigned int getId() {
        return _id;
    }
    /**
        Do nothing, exist solely for making this an abstract class,
        you don't have to override it
    */
    virtual void virtualMethod() {}
};
#endif