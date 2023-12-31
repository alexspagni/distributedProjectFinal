//
// Generated file, do not edit! Created by opp_msgtool 6.0 from shufflerAckCoordinator.msg.
//

#ifndef __SHUFFLERACKCOORDINATOR_M_H
#define __SHUFFLERACKCOORDINATOR_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class shufflerAckCoordinatorMessage;
/**
 * Class generated from <tt>shufflerAckCoordinator.msg:16</tt> by opp_msgtool.
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
 * message shufflerAckCoordinatorMessage
 * {
 *     int reducerSent; //operazione da fare
 *     string partitionToReduce; //partizione da cui leggere
 * 
 * }
 * </pre>
 */
class shufflerAckCoordinatorMessage : public ::omnetpp::cMessage
{
  protected:
    int reducerSent = 0;
    omnetpp::opp_string partitionToReduce;

  private:
    void copy(const shufflerAckCoordinatorMessage& other);

  protected:
    bool operator==(const shufflerAckCoordinatorMessage&) = delete;

  public:
    shufflerAckCoordinatorMessage(const char *name=nullptr, short kind=0);
    shufflerAckCoordinatorMessage(const shufflerAckCoordinatorMessage& other);
    virtual ~shufflerAckCoordinatorMessage();
    shufflerAckCoordinatorMessage& operator=(const shufflerAckCoordinatorMessage& other);
    virtual shufflerAckCoordinatorMessage *dup() const override {return new shufflerAckCoordinatorMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getReducerSent() const;
    virtual void setReducerSent(int reducerSent);

    virtual const char * getPartitionToReduce() const;
    virtual void setPartitionToReduce(const char * partitionToReduce);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const shufflerAckCoordinatorMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, shufflerAckCoordinatorMessage& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline shufflerAckCoordinatorMessage *fromAnyPtr(any_ptr ptr) { return check_and_cast<shufflerAckCoordinatorMessage*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __SHUFFLERACKCOORDINATOR_M_H

