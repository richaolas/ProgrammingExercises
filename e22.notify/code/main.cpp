#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class CNotify
{
public:
    enum SENDTYPE
    {
        SND_SINGLE,
        SND_ALL,
        SND_NOSUPPORT
    };

    CNotify() : m_uid(0) {}

    static void Register(CNotify &rInst)
    {
        rInst.m_uid = GenUUID();
        m_objs.push_back(&rInst);
    }

    static void SendNotify(int iType, unsigned int uID, char *pContent)
    {
        if (SND_SINGLE == iType)
        {
            for (vector<CNotify *>::const_iterator cit = m_objs.begin();
                 cit != m_objs.end(); ++cit)
            {
                if ((*cit)->m_uid == uID)
                {
                    (*cit)->OnNotify(pContent);
                }
            }
        }
        else if (SND_ALL == iType)
        {
            for (vector<CNotify *>::const_iterator cit = m_objs.begin();
                 cit != m_objs.end(); ++cit)
            {
                (*cit)->OnNotify(pContent);
            }
        }
    }

    virtual void OnNotify(char *pContent) const = 0;

    virtual ~CNotify()
    {
        UnRegister();
    }

    int GetUid() const { return m_uid; }

protected:
    CNotify(const CNotify &rInst);
    CNotify &operator=(const CNotify &rInst);

    void UnRegister() const
    {
        for (vector<CNotify *>::const_iterator cit = m_objs.begin();
             cit != m_objs.end(); ++cit)
        {
            if (*cit == this)
            {
                m_objs.erase(cit);
                break;
            }
        }
    }

    static unsigned int GenUUID()
    {
        static unsigned int uid = 1;
        return uid++;
    }

protected:
    static vector<CNotify *> m_objs;

    unsigned int m_uid;
};

vector<CNotify *> CNotify::m_objs;

class CNotifyWorker : public CNotify
{
public:
    virtual void OnNotify(char *pContent) const
    {
        cout << "UID:" << GetUid() << " Content:" << pContent << endl;
    }
};

class CNotifyLier : public CNotify //他收到消息之后，把消息逆向输出
{
public:
    virtual void OnNotify(char *pContent) const
    {
        string s(pContent);
        reverse(s.begin(), s.end());
        cout << "UID:" << GetUid() << " Layer Content:" << s << endl;
    }
};

class CNotifyLazyer : public CNotify
{
private:
    static int cnt;

public:
    CNotifyLazyer() { cnt++; }
    ~CNotifyLazyer() { cnt--; }
    virtual void OnNotify(char *pContent) const
    {
        if (m_objs.size() == cnt)
        {
            cout << "UID:" << GetUid() << " I am Lazyer! " << pContent << endl;
        }
        else
        {
            vector<CNotify *>::iterator it = find(m_objs.begin(), m_objs.end(), this);
            ++it;

            unsigned int sendtouid = 0;

            if (it == m_objs.end())
            {
                sendtouid = (*m_objs.begin())->GetUid();
            }
            else
            {
                sendtouid = (*it)->GetUid();
            }
            CNotify::SendNotify(CNotify::SND_SINGLE, sendtouid, pContent);
        }
    }
};

int CNotifyLazyer::cnt = 0;

int main()
{
    CNotifyWorker c1;
    CNotify::Register(c1);

    CNotifyLier c2;
    CNotify::Register(c2);

    CNotifyLazyer c3;
    CNotify::Register(c3);

    CNotify::SendNotify(CNotify::SND_ALL, 0, (char*)"Message");
    CNotify::SendNotify(CNotify::SND_SINGLE, 3, (char*)"Message");

    return 0;
}
