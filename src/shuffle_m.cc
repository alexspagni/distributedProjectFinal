//
// Generated file, do not edit! Created by opp_msgtool 6.0 from shuffle.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "shuffle_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(ShuffleMessage)

ShuffleMessage::ShuffleMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ShuffleMessage::ShuffleMessage(const ShuffleMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ShuffleMessage::~ShuffleMessage()
{
    delete [] this->partitionElaborated;
}

ShuffleMessage& ShuffleMessage::operator=(const ShuffleMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ShuffleMessage::copy(const ShuffleMessage& other)
{
    this->operationToDo = other.operationToDo;
    this->partitionToRead = other.partitionToRead;
    this->nodeToSend = other.nodeToSend;
    for (size_t i = 0; i < 10; i++) {
        this->reducerAvailable[i] = other.reducerAvailable[i];
    }
    delete [] this->partitionElaborated;
    this->partitionElaborated = (other.partitionElaborated_arraysize==0) ? nullptr : new int[other.partitionElaborated_arraysize];
    partitionElaborated_arraysize = other.partitionElaborated_arraysize;
    for (size_t i = 0; i < partitionElaborated_arraysize; i++) {
        this->partitionElaborated[i] = other.partitionElaborated[i];
    }
    this->createPartition = other.createPartition;
}

void ShuffleMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->operationToDo);
    doParsimPacking(b,this->partitionToRead);
    doParsimPacking(b,this->nodeToSend);
    doParsimArrayPacking(b,this->reducerAvailable,10);
    b->pack(partitionElaborated_arraysize);
    doParsimArrayPacking(b,this->partitionElaborated,partitionElaborated_arraysize);
    doParsimPacking(b,this->createPartition);
}

void ShuffleMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->operationToDo);
    doParsimUnpacking(b,this->partitionToRead);
    doParsimUnpacking(b,this->nodeToSend);
    doParsimArrayUnpacking(b,this->reducerAvailable,10);
    delete [] this->partitionElaborated;
    b->unpack(partitionElaborated_arraysize);
    if (partitionElaborated_arraysize == 0) {
        this->partitionElaborated = nullptr;
    } else {
        this->partitionElaborated = new int[partitionElaborated_arraysize];
        doParsimArrayUnpacking(b,this->partitionElaborated,partitionElaborated_arraysize);
    }
    doParsimUnpacking(b,this->createPartition);
}

const char * ShuffleMessage::getOperationToDo() const
{
    return this->operationToDo.c_str();
}

void ShuffleMessage::setOperationToDo(const char * operationToDo)
{
    this->operationToDo = operationToDo;
}

const char * ShuffleMessage::getPartitionToRead() const
{
    return this->partitionToRead.c_str();
}

void ShuffleMessage::setPartitionToRead(const char * partitionToRead)
{
    this->partitionToRead = partitionToRead;
}

int ShuffleMessage::getNodeToSend() const
{
    return this->nodeToSend;
}

void ShuffleMessage::setNodeToSend(int nodeToSend)
{
    this->nodeToSend = nodeToSend;
}

size_t ShuffleMessage::getReducerAvailableArraySize() const
{
    return 10;
}

int ShuffleMessage::getReducerAvailable(size_t k) const
{
    if (k >= 10) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)10, (unsigned long)k);
    return this->reducerAvailable[k];
}

void ShuffleMessage::setReducerAvailable(size_t k, int reducerAvailable)
{
    if (k >= 10) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)10, (unsigned long)k);
    this->reducerAvailable[k] = reducerAvailable;
}

size_t ShuffleMessage::getPartitionElaboratedArraySize() const
{
    return partitionElaborated_arraysize;
}

int ShuffleMessage::getPartitionElaborated(size_t k) const
{
    if (k >= partitionElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionElaborated_arraysize, (unsigned long)k);
    return this->partitionElaborated[k];
}

void ShuffleMessage::setPartitionElaboratedArraySize(size_t newSize)
{
    int *partitionElaborated2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = partitionElaborated_arraysize < newSize ? partitionElaborated_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        partitionElaborated2[i] = this->partitionElaborated[i];
    for (size_t i = minSize; i < newSize; i++)
        partitionElaborated2[i] = 0;
    delete [] this->partitionElaborated;
    this->partitionElaborated = partitionElaborated2;
    partitionElaborated_arraysize = newSize;
}

void ShuffleMessage::setPartitionElaborated(size_t k, int partitionElaborated)
{
    if (k >= partitionElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionElaborated_arraysize, (unsigned long)k);
    this->partitionElaborated[k] = partitionElaborated;
}

void ShuffleMessage::insertPartitionElaborated(size_t k, int partitionElaborated)
{
    if (k > partitionElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionElaborated_arraysize, (unsigned long)k);
    size_t newSize = partitionElaborated_arraysize + 1;
    int *partitionElaborated2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        partitionElaborated2[i] = this->partitionElaborated[i];
    partitionElaborated2[k] = partitionElaborated;
    for (i = k + 1; i < newSize; i++)
        partitionElaborated2[i] = this->partitionElaborated[i-1];
    delete [] this->partitionElaborated;
    this->partitionElaborated = partitionElaborated2;
    partitionElaborated_arraysize = newSize;
}

void ShuffleMessage::appendPartitionElaborated(int partitionElaborated)
{
    insertPartitionElaborated(partitionElaborated_arraysize, partitionElaborated);
}

void ShuffleMessage::erasePartitionElaborated(size_t k)
{
    if (k >= partitionElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionElaborated_arraysize, (unsigned long)k);
    size_t newSize = partitionElaborated_arraysize - 1;
    int *partitionElaborated2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        partitionElaborated2[i] = this->partitionElaborated[i];
    for (i = k; i < newSize; i++)
        partitionElaborated2[i] = this->partitionElaborated[i+1];
    delete [] this->partitionElaborated;
    this->partitionElaborated = partitionElaborated2;
    partitionElaborated_arraysize = newSize;
}

bool ShuffleMessage::getCreatePartition() const
{
    return this->createPartition;
}

void ShuffleMessage::setCreatePartition(bool createPartition)
{
    this->createPartition = createPartition;
}

class ShuffleMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_operationToDo,
        FIELD_partitionToRead,
        FIELD_nodeToSend,
        FIELD_reducerAvailable,
        FIELD_partitionElaborated,
        FIELD_createPartition,
    };
  public:
    ShuffleMessageDescriptor();
    virtual ~ShuffleMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ShuffleMessageDescriptor)

ShuffleMessageDescriptor::ShuffleMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ShuffleMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ShuffleMessageDescriptor::~ShuffleMessageDescriptor()
{
    delete[] propertyNames;
}

bool ShuffleMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ShuffleMessage *>(obj)!=nullptr;
}

const char **ShuffleMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ShuffleMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ShuffleMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int ShuffleMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_operationToDo
        FD_ISEDITABLE,    // FIELD_partitionToRead
        FD_ISEDITABLE,    // FIELD_nodeToSend
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_reducerAvailable
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_partitionElaborated
        FD_ISEDITABLE,    // FIELD_createPartition
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *ShuffleMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "operationToDo",
        "partitionToRead",
        "nodeToSend",
        "reducerAvailable",
        "partitionElaborated",
        "createPartition",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int ShuffleMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "operationToDo") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "partitionToRead") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "nodeToSend") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "reducerAvailable") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "partitionElaborated") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "createPartition") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *ShuffleMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_operationToDo
        "string",    // FIELD_partitionToRead
        "int",    // FIELD_nodeToSend
        "int",    // FIELD_reducerAvailable
        "int",    // FIELD_partitionElaborated
        "bool",    // FIELD_createPartition
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **ShuffleMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ShuffleMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ShuffleMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducerAvailable: return 10;
        case FIELD_partitionElaborated: return pp->getPartitionElaboratedArraySize();
        default: return 0;
    }
}

void ShuffleMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_partitionElaborated: pp->setPartitionElaboratedArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ShuffleMessage'", field);
    }
}

const char *ShuffleMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ShuffleMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_operationToDo: return oppstring2string(pp->getOperationToDo());
        case FIELD_partitionToRead: return oppstring2string(pp->getPartitionToRead());
        case FIELD_nodeToSend: return long2string(pp->getNodeToSend());
        case FIELD_reducerAvailable: return long2string(pp->getReducerAvailable(i));
        case FIELD_partitionElaborated: return long2string(pp->getPartitionElaborated(i));
        case FIELD_createPartition: return bool2string(pp->getCreatePartition());
        default: return "";
    }
}

void ShuffleMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_operationToDo: pp->setOperationToDo((value)); break;
        case FIELD_partitionToRead: pp->setPartitionToRead((value)); break;
        case FIELD_nodeToSend: pp->setNodeToSend(string2long(value)); break;
        case FIELD_reducerAvailable: pp->setReducerAvailable(i,string2long(value)); break;
        case FIELD_partitionElaborated: pp->setPartitionElaborated(i,string2long(value)); break;
        case FIELD_createPartition: pp->setCreatePartition(string2bool(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShuffleMessage'", field);
    }
}

omnetpp::cValue ShuffleMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_operationToDo: return pp->getOperationToDo();
        case FIELD_partitionToRead: return pp->getPartitionToRead();
        case FIELD_nodeToSend: return pp->getNodeToSend();
        case FIELD_reducerAvailable: return pp->getReducerAvailable(i);
        case FIELD_partitionElaborated: return pp->getPartitionElaborated(i);
        case FIELD_createPartition: return pp->getCreatePartition();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ShuffleMessage' as cValue -- field index out of range?", field);
    }
}

void ShuffleMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        case FIELD_operationToDo: pp->setOperationToDo(value.stringValue()); break;
        case FIELD_partitionToRead: pp->setPartitionToRead(value.stringValue()); break;
        case FIELD_nodeToSend: pp->setNodeToSend(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_reducerAvailable: pp->setReducerAvailable(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_partitionElaborated: pp->setPartitionElaborated(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_createPartition: pp->setCreatePartition(value.boolValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShuffleMessage'", field);
    }
}

const char *ShuffleMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ShuffleMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ShuffleMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ShuffleMessage *pp = omnetpp::fromAnyPtr<ShuffleMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShuffleMessage'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

