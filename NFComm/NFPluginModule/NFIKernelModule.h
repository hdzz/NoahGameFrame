// -------------------------------------------------------------------------
//    @FileName         :    NFIKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIKernelModule
//
// -------------------------------------------------------------------------

#ifndef NFI_KERNEL_MODULE_H
#define NFI_KERNEL_MODULE_H

#include <iostream>
#include <string>
#include <functional>
#include "NFIModule.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFKernelPlugin/NFIScheduleModule.h"

class NFIKernelModule
    : public NFIModule
{

public:

	template<typename BaseType>
	bool AddSchedule(const NFGUID self, const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fTime, const int nCount)
	{
		GetScheduleModule()->AddSchedule(self, strScheduleName, pBase, handler, fTime, nCount);

		return false;
	}

	virtual NFIScheduleModule* GetScheduleModule() = 0;

	virtual bool FindSchedule(const NFGUID& self, const std::string& strScheduleName) = 0;

	virtual bool RemoveSchedule(const NFGUID& self, const std::string& strScheduleName) = 0;

    template<typename BaseType>
    bool AddRecordCallBack(const NFGUID& self, const std::string& strRecordName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
        if (pObject.get())
        {
            return pObject->AddRecordCallBack(strRecordName, pBase, handler);
        }

        return false;
    }

    template<typename BaseType>
    bool AddPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        NF_SHARE_PTR<NFIObject> pObject = GetObject(self);
        if (pObject.get())
        {
            return pObject->AddPropertyCallBack(strPropertyName, pBase, handler);
        }

        return false;
    }

    ////////////////event//////////////////////////////////////////////////////////
    template<typename BaseType>
    bool AddEventCallBack(const NFGUID& self, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const NFIDataList&))
    {
        EVENT_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        EVENT_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_PROCESS_FUNCTOR(functor));
        return AddEventCallBack(self, nEventID, functorPtr);
    }

    template<typename BaseType>
    bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(strClassName, functorPtr);
    }

    virtual bool DoEvent(const NFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const NFIDataList& valueList) = 0;
    virtual bool DoEvent(const NFGUID& self, const int nEventID, const NFIDataList& valueList) = 0;

    //////////////////////////////////////////////////////////////////////////
    //ֻ������ģ��ע�ᣬ�ص�����ͬ���������¼�,���е������󶼻��ص�
    template<typename BaseType>
    bool RegisterCommonClassEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return RegisterCommonClassEvent(functorPtr);
    }

    //ֻ������ģ��ע�ᣬ�ص�����ͬ�����������¼�,���е������Զ����ص�
    template<typename BaseType>
    bool RegisterCommonPropertyEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        PROPERTY_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_EVENT_FUNCTOR(functor));
        return RegisterCommonPropertyEvent(functorPtr);
    }

    //ֻ������ģ��ע�ᣬ�ص�����ͬ�����������¼�,���е����������ص�
    template<typename BaseType>
    bool RegisterCommonRecordEvent(BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const RECORD_EVENT_DATA&, const NFIDataList::TData&, const NFIDataList::TData&))
    {
        RECORD_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(new RECORD_EVENT_FUNCTOR(functor));
        return RegisterCommonRecordEvent(functorPtr);
    }


    /////////////////////////////////////////////////////////////////


    virtual bool IsContainer(const NFGUID& self) = 0;
    virtual bool ExistContainer(const int nContainerIndex) = 0;

    virtual NF_SHARE_PTR<NFIObject> GetObject(const NFGUID& ident) = 0;
    virtual NF_SHARE_PTR<NFIObject> CreateObject(const NFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIDataList& arg) = 0;

    virtual bool DestroyObject(const NFGUID& self) = 0;
    virtual bool DestroyAll() = 0;

    virtual bool FindProperty(const NFGUID& self, const std::string& strPropertyName) = 0;

    virtual bool SetPropertyInt(const NFGUID& self, const std::string& strPropertyName, const NFINT64 nValue) = 0;
    virtual bool SetPropertyFloat(const NFGUID& self, const std::string& strPropertyName, const double dValue) = 0;
    virtual bool SetPropertyString(const NFGUID& self, const std::string& strPropertyName, const std::string& strValue) = 0;
    virtual bool SetPropertyObject(const NFGUID& self, const std::string& strPropertyName, const NFGUID& objectValue) = 0;

    virtual NFINT64 GetPropertyInt(const NFGUID& self, const std::string& strPropertyName) = 0;
    virtual double GetPropertyFloat(const NFGUID& self, const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const NFGUID& self, const std::string& strPropertyName) = 0;
    virtual const NFGUID& GetPropertyObject(const NFGUID& self, const std::string& strPropertyName) = 0;

    virtual NF_SHARE_PTR<NFIRecord> FindRecord(const NFGUID& self, const std::string& strRecordName) = 0;
    virtual bool ClearRecord(const NFGUID& self, const std::string& strRecordName) = 0;

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue) = 0;
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& objectValue) = 0;

    virtual bool SetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
    virtual bool SetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value) = 0;
    virtual bool SetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value) = 0;
    virtual bool SetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value) = 0;

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual NFINT64 GetRecordInt(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual double GetRecordFloat(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual const std::string& GetRecordString(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual const NFGUID& GetRecordObject(const NFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;

    virtual bool SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIDataList& arg) = 0;

    ////////////////////////////////////////////////////////////////
    virtual NFGUID CreateGUID() = 0;

    virtual bool CreateScene(const int nSceneID) = 0;
    virtual bool DestroyScene(const int nSceneID) = 0;

    virtual int GetOnLineCount() = 0;
    virtual int GetMaxOnLineCount() = 0;
    virtual int GetSceneOnLineCount(const int nSceneID) = 0;
    virtual int GetSceneOnLineCount(const int nSceneID, const int nGroupID) = 0;
    virtual int GetSceneOnLineList(const int nSceneID, NFIDataList& var) = 0;

    virtual int RequestGroupScene(const int nSceneID) = 0;
    virtual bool ReleaseGroupScene(const int nSceneID, const int nGroupID) = 0;
    virtual bool ExitGroupScene(const int nSceneID, const int nGroupID) = 0;

    virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, NFIDataList& list) = 0;
    virtual int GetObjectByProperty(const int nSceneID, const std::string& strPropertyName, const NFIDataList& valueArg, NFIDataList& list) = 0;

    virtual void Random(int nStart, int nEnd, int nCount, NFIDataList& valueList) = 0;
    virtual bool LogInfo(const NFGUID ident) = 0;

	virtual NFINT64 GetTime() = 0;

protected:
    virtual bool AddEventCallBack(const NFGUID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;


protected:

    //ֻ������ģ��ע�ᣬ�ص�����ͬ���������¼�,���е������󶼻��ص�
    virtual bool RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

    //ֻ������ģ��ע�ᣬ�ص�����ͬ�����������¼�,���е������Զ����ص�
    virtual bool RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

    //ֻ������ģ��ע�ᣬ�ص�����ͬ�����������¼�,���е����������ص�
    virtual bool RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;
};

#endif
