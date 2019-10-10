#pragma once

#ifndef __RPC_WEB_CLIENT_H__
#define __RPC_WEB_CLIENT_H__

#include "RpcClient.h"
#include "webcore_h.h"


class RpcClient;
class RpcWebClient;

class RpcWebClient: public RpcClient {

public:
    enum tag_eConnType {

        eConnTypeView   = 0,
        eConnTypeDraw   = 2
    };

private:
    enum {

        eConnIdInvalid  = 0
    };

private:
    tag_eConnType       m_connType;
    long                m_connId;
    bool                m_fIsAuthenticated;
    bool                m_fIsSvcProtected;
    std::string         m_code;

public:
    RpcWebClient();
    RpcWebClient(__in std::string &svcAddress, __in std::string &svcEndpoint, __in tag_eConnType connType);
    ~RpcWebClient();

public:
    virtual bool Connect();
    virtual void Disconnect();
    bool Configure(__in std::string &svcAddress, __in std::string &svcEndpoint, __in tag_eConnType connType);
    bool ExecuteRequest(__in long ioctl, __in long sizeIn, __in byte *buffIn, __in long sizeOut, __out byte *buffOut, __out long *pResultOut);

protected:
    virtual bool _ConnectionProbe();
    virtual void _ConnectionPost();

public:
    bool InqIsAuthenticated();
    bool InqIsSvcProtected();
    void InqCode(__out std::string &codeOut);

};

#endif