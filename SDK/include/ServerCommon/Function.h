#pragma once

//��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//��ȫɾ��ָ��
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { ERROR_LOG("CATCH:%s with %s\n",__FILE__,__FUNCTION__);} pData = NULL; } 

//��ȫɾ��ָ��
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { ERROR_LOG("CATCH:%s with %s\n",__FILE__,__FUNCTION__);} pData = NULL; } 

//��ȫɾ��ָ��
#define SAFE_DELETE(ptr) { if(ptr){	try{ delete ptr; }catch(...){ ERROR_LOG("CATCH: *** SAFE_DELETE(%s) crash! *** %s %d\n",#ptr,__FILE__, __LINE__); } ptr = 0; } }