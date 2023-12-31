//
// Generated file, do not edit! Created by opp_msgtool 6.0 from shuffle.msg.
//

#ifndef __SHUFFLE_M_H
#define __SHUFFLE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class ShuffleMessage;
/**
 * Class generated from <tt>shuffle.msg:16</tt> by opp_msgtool.
 * <pre>
 * //
 * // This program is free software: you can redistribute it and/or modify
 * // it under the terms of the GNU Lesser General Public License as published by
 * // the Free Software Foundation, either version 3 of the License, or
 * // (at your option) any later version.
 * // 
 * // This program is distributed in the hope that it will be useful,
 * // but WITHOUT ANY WARRANTY; without even the implied warranty of
 * // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * // GNU Lesser General Public License for more details.
 * // 
 * // You should have received a copy of the GNU Lesser General Public License
 * // along with this program.  If not, see http://www.gnu.org/licenses/.
 * //
 * message ShuffleMessage
 * {
 *     string operationToDo; //operazione da fare
 *     string partitionToRead; //partizione da cui leggere
 *     int nodeToSend; // nodo a cui inviare
 *     int reducerAvailable[10]; // reducer disponibili
 *     int partitionElaborated[]; //partizioni che devono essere elaborate
 *     bool createPartition;
 * 
 * }
 * </pre>
 */
class ShuffleMessage : public ::omnetpp::cMessage
{
  protected:
    omnetpp::opp_string operationToDo;
    omnetpp::opp_string partitionToRead;
    int nodeToSend = 0;
    int reducerAvailable[10] = {0};
    int *partitionElaborated = nullptr;
    size_t partitionElaborated_arraysize = 0;
    bool createPartition = false;

  private:
    void copy(const ShuffleMessage& other);

  protected:
    bool operator==(const ShuffleMessage&) = delete;

  public:
    ShuffleMessage(const char *name=nullptr, short kind=0);
    ShuffleMessage(const ShuffleMessage& other);
    virtual ~ShuffleMessage();
    ShuffleMessage& operator=(const ShuffleMessage& other);
    virtual ShuffleMessage *dup() const override {return new ShuffleMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual const char * getOperationToDo() const;
    virtual void setOperationToDo(const char * operationToDo);

    virtual const char * getPartitionToRead() const;
    virtual void setPartitionToRead(const char * partitionToRead);

    virtual int getNodeToSend() const;
    virtual void setNodeToSend(int nodeToSend);

    virtual size_t getReducerAvailableArraySize() const;
    virtual int getReducerAvailable(size_t k) const;
    virtual void setReducerAvailable(size_t k, int reducerAvailable);

    virtual void setPartitionElaboratedArraySize(size_t size);
    virtual size_t getPartitionElaboratedArraySize() const;
    virtual int getPartitionElaborated(size_t k) const;
    virtual void setPartitionElaborated(size_t k, int partitionElaborated);
    virtual void insertPartitionElaborated(size_t k, int partitionElaborated);
    [[deprecated]] void insertPartitionElaborated(int partitionElaborated) {appendPartitionElaborated(partitionElaborated);}
    virtual void appendPartitionElaborated(int partitionElaborated);
    virtual void erasePartitionElaborated(size_t k);

    virtual bool getCreatePartition() const;
    virtual void setCreatePartition(bool createPartition);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ShuffleMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ShuffleMessage& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline ShuffleMessage *fromAnyPtr(any_ptr ptr) { return check_and_cast<ShuffleMessage*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __SHUFFLE_M_H

