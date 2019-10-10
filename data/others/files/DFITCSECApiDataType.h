/**
 * ��Ȩ����(C)2012-2016, �����ɴ���Ϣ�������޹�˾
 * �ļ����ƣ�DFITCSECApiDataType.h
 * �ļ�˵��������ӿ�������������͵�ͷ�ļ�
 * ��ǰ�汾��1.6
 * ���ߣ�XSpeed֤ȯ��Ŀ��
 * �������ڣ�2016��08��
 */

#ifndef DFITCSECAPIDATATYPE_H
#define DFITCSECAPIDATATYPE_H

/////////////////////////////////////////////////////////////////////////
///DFITCSECBusinessTypeType ��һ��ҵ���������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECBusinessTypeType;
///��Ʊ
#define DFITCSEC_BT_Stock                                         0
///ETF��Ȩ
#define DFITCSEC_BT_SOP                                           1
///������ȯ
#define DFITCSEC_BT_FASL                                          2

/////////////////////////////////////////////////////////////////////////
///DFITCSECRequestIDType ��һ������ID����
/////////////////////////////////////////////////////////////////////////
typedef long   DFITCSECRequestIDType; 

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountIDType ��һ���ͻ�������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECAccountIDType[21];

/////////////////////////////////////////////////////////////////////////
///DFITCSECPasswordType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECPasswordType[17];

/////////////////////////////////////////////////////////////////////////
///DFITCSECErrorIDType ��һ������ID����
/////////////////////////////////////////////////////////////////////////
typedef long   DFITCSECErrorIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECSessionIDType ��һ���ỰID����
/////////////////////////////////////////////////////////////////////////
typedef long   DFITCSECSessionIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECLocalOrderIDType ��һ������ί�к�����
/////////////////////////////////////////////////////////////////////////
typedef long   DFITCSECLocalOrderIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECSpdOrderIDType ��һ����̨ί�к�����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECSpdOrderIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECMessageType ��һ������Ϣ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECMessageType[1024];

/////////////////////////////////////////////////////////////////////////
///DFITCSECFrontIDType ��һ��ǰ�û��������
/////////////////////////////////////////////////////////////////////////
typedef long   DFITCSECFrontIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECTimeType ��һ��ʱ������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECTimeType[13];

/////////////////////////////////////////////////////////////////////////
///DFITCSECDateType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECDateType;
/////////////////////////////////////////////////////////////////////////
///DFITCSECWeekType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECWeekType;
/////////////////////////////////////////////////////////////////////////
///DFITCSECDateType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECMillisecondType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECPasswordTypeType ��һ�������������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECPasswordTypeType;
///����
#define DFITCSEC_PWT_Trade                                        1
///�ʽ�
#define DFITCSEC_PWT_Funds                                        2

/////////////////////////////////////////////////////////////////////////
///DFITCSECPriceType ��һ���۸�����
/////////////////////////////////////////////////////////////////////////
typedef double DFITCSECPriceType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECFundsType ��һ���������
/////////////////////////////////////////////////////////////////////////
typedef double DFITCSECFundsType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECPositionSourceType ��һ��ͷ����Դ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECPositionSourceType;
///��ͨͷ��
#define DFITCSEC_PS_General                                       0
///ר��ͷ��
#define DFITCSEC_PS_Special                                       1

/////////////////////////////////////////////////////////////////////////
///DFITCSECEntrustDirectionType ��һ��ί���������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECEntrustDirectionType;
///��
#define DFITCSEC_ED_Buy                                           1
///��
#define DFITCSEC_ED_Sell                                          2
///��ɽɿ�
#define DFITCSEC_ED_RightsIssueContribution                       3
///�ع�����
#define DFITCSEC_ED_RepurchaseFinancing                           4
///�ع���ȯ
#define DFITCSEC_ED_RepurchaseSecurities                          5
///����ת�й�
#define DFITCSEC_ED_CustodyTransfer                               7
///�Ϻ�ָ������
#define DFITCSEC_ED_DesignateTrading                              9
///�Ϻ���ָ����
#define DFITCSEC_ED_RevocationDesignateTrading                    10
///ծת��
#define DFITCSEC_ED_DebtToEquity                                  11
///תծ����
#define DFITCSEC_ED_BondsSoldBack                                 12
///�����깺
#define DFITCSEC_ED_PlacingPurchase                               14
///ͶƱ
#define DFITCSEC_ED_Vote                                          23
///ETF�깺
#define DFITCSEC_ED_Purchase                                      29
///ETF���
#define DFITCSEC_ED_Redemp                                        30
///��Ѻȯ���
#define DFITCSEC_ED_PledgeVoucherIn                               37
///��Ѻȯ����
#define DFITCSEC_ED_PledgeVoucherOut                              38
///�����Ϲ�
#define DFITCSEC_ED_FundSubscribe                                 41
///�����깺
#define DFITCSEC_ED_FundPurchase                                  42
///�������
#define DFITCSEC_ED_FundRedemption                                43
///����ֺ�����
#define DFITCSEC_ED_FundBonusSetting                              44
///����ת��
#define DFITCSEC_ED_FundConversion                                46
///����ֲ�
#define DFITCSEC_ED_FundSplit                                     47
///����ϲ�
#define DFITCSEC_ED_FundMerge                                     48
///Ȩ֤��Ȩ
#define DFITCSEC_ED_WarrantExercise                               50
///��������
#define DFITCSEC_ED_FinancingToBuy                                61
///��ȯ����
#define DFITCSEC_ED_SellSecPayment                                62
///��ȯ��ȯ
#define DFITCSEC_ED_BuySecRepaySec                                63
///��ȯ����
#define DFITCSEC_ED_SecuritiesToSell                              64
///��������
#define DFITCSEC_ED_GuaranteeInto                                 65
///��������
#define DFITCSEC_ED_GuaranteeLayOff                               66
///����ǿƽ
#define DFITCSEC_ED_FinancingForceSelling                         71
///��ȯǿƽ
#define DFITCSEC_ED_SecuritiesForceSelling                        72
///Ԥ��ҪԼ
#define DFITCSEC_ED_AcceptOffer                                   76
///���Ԥ��ҪԼ
#define DFITCSEC_ED_GiveUpAcceptOffer                             77

/////////////////////////////////////////////////////////////////////////
///DFITCSECQuantityType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECQuantityType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECLargeQuantityType ��һ������������
/////////////////////////////////////////////////////////////////////////
#ifdef WIN32
typedef __int64 DFITCSECLargeQuantityType;
#else
typedef long   DFITCSECLargeQuantityType;
#endif

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderTypeType ��һ�������������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOrderTypeType;
////STOCK���ֵĶ�������
///�޼�
#define DFITCSEC_OT_LimitPrice                                    0
///�����嵵�����ɽ�ʣ�೷�����Ϻ���
#define DFITCSEC_OT_SHBESTFRTradeLeftWithdraw                     1
///�����嵵�����ɽ�ʣ��ת�޼ۣ��Ϻ���
#define DFITCSEC_OT_SHBESTFRTradeLeftTLimit                       2
///�Է����ż۸����ڣ�
#define DFITCSEC_OT_SZOtherBestPrice                              101
///�������ż۸����ڣ�
#define DFITCSEC_OT_SZBestPrice                                   102
///��ʱ�ɽ�ʣ�೷�������ڣ�
#define DFITCSEC_OT_SZImdeTradeLeftWithdraw                       103
///�����嵵��ʱ�ɽ�ʣ�೷�������ڣ�
#define DFITCSEC_OT_SZBESTFRTradeLeftWithdraw                     104
///ȫ��ɽ�����
#define DFITCSEC_OT_SZBESTTotalTradeOWithdraw                     105
////SOP���ֵĶ�������
///�޼�
#define DFITCSEC_SOP_LimitPrice                                   1
///�м�
#define DFITCSEC_SOP_LastPrice                                    2
///�м�ʣ��ת�޼ۣ��Ϻ���
#define DFITCSEC_SOP_LastTLimit                                   3
///�м۱������ţ����ڣ�
#define DFITCSEC_SOP_MineBestPrice                                4
///�м������嵵�ɽ�ʣ�೷�������ڣ�
#define DFITCSEC_SOP_OptimalFive                                  5

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountType ��һ���ͻ�����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountType;
///��˾
#define DFITCSEC_AT_Company                                       0
///����
#define DFITCSEC_AT_Personal                                      1

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderBatchIDType ��һ��ί�����κ�����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECEntrustBatchIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECSeatIDType ��һ��ϯλ������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECSeatIDType[9];

/////////////////////////////////////////////////////////////////////////
///DFITCSECExchangeType ��һ����������������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECExchangeIDType[3];
///��A
#define DFITCSEC_EI_SH                                          "SH"
///��A
#define DFITCSEC_EI_SZ                                          "SZ"

/////////////////////////////////////////////////////////////////////////
///DFITCSecurityIDType ��һ��֤ȯ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECSecurityIDType[11];

////////////////////////////////////////////////////////////
///DFITCSECOrderQryFlagType ��һ��������ѯ��־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECEntrustQryFlagType;
///����ί��
#define DFITCSTOCK_EQF_Total                                    0
///�ɳ���ί��
#define DFITCSTOCK_EQF_AbleWithdraw                             1

/////////////////////////////////////////////////////////////////////////
///DFITCSECWithdrawFlagType ��һ��������־����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECWithdrawFlagType[2];
///ί��
#define DFITCSEC_CF_EntrustSucc                                 "O"
///����
#define DFITCSEC_CF_WithdrawSuc                                 "W"
////FASL���ú�Լ�䶯��Ϣ��ѯר��
///����
#define DFITCSEC_CF_Withdraw                                    "-1"
///����
#define DFITCSEC_CF_Normal                                      "0"

/////////////////////////////////////////////////////////////////////////
///DFITCSECIncQryIndexType ��һ��������ѯ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECIncQryIndexType[129];

////////////////////////////////////////////////////////////
///DFITCSECTradeQryFlagType ��һ���ɽ���ѯ��־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECTradeQryFlagType;
///���л�ת��¼��������ת
#define DFITCSEC_TQF_Total                                      0
///���سɽ���¼
#define DFITCSEC_TQF_Trade                                      1

////////////////////////////////////////////////////////////
///DFITCSECPositionQryFlagType ��һ���ɽ���ѯ��־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECPositionQryFlagType;
///����չ��Ϣ
#define DFITCSEC_PQF_Normal                                     0
///������չ��Ϣ
#define DFITCSEC_PQF_Extend                                     1

////////////////////////////////////////////////////////////
///DFITCSECFundsQryFlagType ��һ���ʽ��ѯ��־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECFundsQryFlagType;
///���ػ�����Ϣ
#define DFITCSEC_FQF_Normal                                     0
///��չ�ֶΣ��ݲ�ʹ��
#define DFITCSEC_FQF_Extend                                     1

////////////////////////////////////////////////////////////
///DFITCSECSystemQryFlagType ��һ���߾��ȷ��ر�־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECSystemQryFlagType;
///����ʱ��
#define DFITCSEC_PQF_Normal                                     0
///�߾���ʱ��
#define DFITCSEC_SQF_High                                       1

/////////////////////////////////////////////////////////////////////////
///DFITCSECCurrencyType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECCurrencyType[4];
///�����
#define DFITCSEC_C_RMB                                          "RMB"
///��Ԫ 
#define DFITCSEC_S_USD                                          "USD"
///�۱�
#define DFITCSEC_S_HKD                                          "HKD"

////////////////////////////////////////////////////////////
///DFITCSECFundsTransferFlagType ��һ���ʽ��ת��־����
////////////////////////////////////////////////////////////
typedef int    DFITCSECFundsTransferFlagType;
///ת��
#define DFITCSEC_FTF_In                                         1
///ת�� 
#define DFITCSEC_FTF_Out                                        2

/////////////////////////////////////////////////////////////////////////
///DFITCSECEntrustBatchOrderDetailType ��һ������ί����ϸ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECEntrustBatchOrderDetailType[1024];

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderRangeID ��һ��ί�кŷ�Χ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECOrderIDRangeType[1024];

/////////////////////////////////////////////////////////////////////////
///DFITCSECFundsFreezeTypeType ��һ���ʽ𶳽��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECFundsFreezeTypeType;
///ί�ж���
#define DFITCSEC_FFT_EntrustFreeze                             1
///ʵʱ�ɽ��ⶳ
#define DFITCSEC_FFT_RealtimeTradeUnFreeze                     2
///�ֹ�����
#define DFITCSEC_FFT_ManualFreeze                              3
///�ֹ��ⶳ
#define DFITCSEC_FFT_ManualUnFreeze                            4

/////////////////////////////////////////////////////////////////////////
///DFITCSECTransFundsFreezeTypeType ��һ�������ʽ𶳽��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECTransFundsFreezeTypeType;
///��ʼ���ʽ�
#define DFITCSEC_FFT_InitFunds                                 50
///�����ʽ����
#define DFITCSEC_FFT_FundsIn                                   51
///�����ʽ����
#define DFITCSEC_FFT_FundsOut                                  52

/////////////////////////////////////////////////////////////////////////
///DFITCSECSerialIDType ��һ����ˮ������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECSerialIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECStockFreezeTypeType ��һ��֤ȯ�����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECStockFreezeTypeType;
///ί�ж���
#define DFITCSEC_SFT_EntrustFreeze                            1
///���ӿ�������
#define DFITCSEC_SFT_IncSellQty                               2
///���ٿ�������
#define DFITCSEC_SFT_DecSellQty                               3
///�ֹ����ڶ���
#define DFITCSEC_SFT_ManualFreeze                             4
///���㶳��
#define DFITCSEC_SFT_ClearFreeze                              5
///ETF�깺��ض���
#define DFITCSEC_SFT_PurchaseETFFreeze                        6
///ҪԼ�չ�����
#define DFITCSEC_SFT_OfferFreeze                              7
///Ȩ֤��Ȩ����
#define DFITCSEC_SFT_ExeFreeze                                8
///ծȯ��Ѻת��׼ȯ����
#define DFITCSEC_SFT_CollTranStandFreeze                      9
///��Ѻȯ��ⶳ��
#define DFITCSEC_SFT_PledgeStoringFreeze                      10
///��ʼ���ɷ�
#define DFITCSEC_SFT_INITStock                                50
///����֤ȯ����
#define DFITCSEC_SFT_StockIn                                  53
///����֤ȯ����
#define DFITCSEC_SFT_StockOut                                 54

/////////////////////////////////////////////////////////////////////////
///DFITCSECShareholderIDType ��һ���ɶ�������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECShareholderIDType[11];

/////////////////////////////////////////////////////////////////////////
///DFITCSECEntrustTypeType ��һ��ί�з�ʽ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECEntrustTypeType;
///�绰
#define DFITCSEC_ET_Phone                                    1
///�ſ�
#define DFITCSEC_ET_MagicCard                                2
///�ȼ�
#define DFITCSEC_ET_Hotkey                                   4
///��̨
#define DFITCSEC_ET_Spd                                      8
///Զ��
#define DFITCSEC_ET_Remote                                   16
///������
#define DFITCSEC_ET_Internet                                 32
///�ֻ�
#define DFITCSEC_ET_Mobile                                   64

/////////////////////////////////////////////////////////////////////////
///DFITCSECSecurityNameType ��һ��֤ȯ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECSecurityNameType[21];

/////////////////////////////////////////////////////////////////////////
///DFITCSECDeclareResultType ��һ���걨�������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECDeclareResultType;
///δ�걨
#define DFITCSEC_DR_UnDeclare                                0
///�����걨
#define DFITCSEC_DR_Declaring                                1
///���걨δ�ɽ�
#define DFITCSEC_DR_UnTrade                                  2
///�Ƿ�ί��/�ϵ�
#define DFITCSEC_DR_EntrustFail                              3
///��������ȷ��
#define DFITCSEC_DR_Confirm                                  4
///���ֳɽ�
#define DFITCSEC_DR_PartTrade                                5
///ȫ���ɽ�
#define DFITCSEC_DR_TotalTrade                               6
///���ɲ���
#define DFITCSEC_DR_TradeAWithdraw                           7
///ȫ������
#define DFITCSEC_DR_TotalWithdraw                            8
///����δ��
#define DFITCSEC_DR_WithdrawFail                             9
///�ȴ��˹��걨
#define DFITCSEC_DR_ManualDeclare                            10

/////////////////////////////////////////////////////////////////////////
///DFITCSECBatchDeclareResultType ��һ�������걨�������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECBatchDeclareResultType;
///���걨
#define DFITCSEC_BDR_Declared                                1

/////////////////////////////////////////////////////////////////////////
///DFITCSECDeclareOrderIDType ���걨ί�к�����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECDeclareOrderIDType[11];

/////////////////////////////////////////////////////////////////////////
///DFITCSECIPAddressType ��һ��ip��ַ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECIPAddressType[32];

/////////////////////////////////////////////////////////////////////////
///DFITCSECMacAddressType ��һ��mac��ַ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECMacAddressType[32];

/////////////////////////////////////////////////////////////////////////
///DFITCSECTradeIDType ��һ���ɽ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECTradeIDType[17];

/////////////////////////////////////////////////////////////////////////
///DFITCSECSecurityTypeType ��һ��֤ȯ�������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECSecurityTypeType[3];

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountStatusType ��һ��״̬��־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountStatusType;
///����
#define DFITCSEC_AS_Normal                                    0
///����
#define DFITCSEC_AS_Freeze                                    1
///�ͻ�����ʧ
#define DFITCSEC_AS_LossReporting                             2
///����
#define DFITCSEC_AS_CloseAccount                              3
///С������
#define DFITCSEC_AS_MicroDormancy                             6
///���ϸ�
#define DFITCSEC_AS_UnQualified                               7
///��˾���ϸ�
#define DFITCSEC_AS_CompUnQualified                           9

/////////////////////////////////////////////////////////////////////////
///DFITCSECTradeStatusType ��һ��״̬��־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECTradeStatusType;
///����
#define DFITCSEC_TS_Normal                                    0
///��ͣ
#define DFITCSEC_TS_Suspend                                   2
///����
#define DFITCSEC_TS_CallOut                                   4

/////////////////////////////////////////////////////////////////////////
///DFITCSECBranchIDType ��һ��������������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECBranchIDType[5];

/////////////////////////////////////////////////////////////////////////
///DFITCSECPhoneIDType ��һ����ϵ�绰����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECPhoneIDType[21];

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountNameType ��һ���ͻ���������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECAccountNameType[81];

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountIdentityIDType ��һ��֤���������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECAccountIdentityIDType[31];

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountIdentityTypeType ��һ��֤����ʾ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountIdentityTypeType;
///���֤
#define DFITCSEC_AI_IDcard                                   0
///����
#define DFITCSEC_AI_PasPrt                                   1
///����֤
#define DFITCSEC_AI_OffCard                                  2
///ʿ��֤
#define DFITCSEC_AI_SolderCard                               3
///����֤
#define DFITCSEC_AI_RntryPrmt                                4
///���ڱ�
#define DFITCSEC_AI_HusHldRgstr                              5
///�������
#define DFITCSEC_AI_FrignPasPrt                              6
///�����ල�ִ���
#define DFITCSEC_AI_TechSupBruCode                           7
///Ӫҵִ��
#define DFITCSEC_AI_BsnsLicence                              8
///��������
#define DFITCSEC_AI_AdminOrgan                               9
///�������
#define DFITCSEC_AI_SocialGrup                               10
///����
#define DFITCSEC_AI_Army                                     11
///�侯
#define DFITCSEC_AI_ArmdPolice                               12
///��������
#define DFITCSEC_AI_SubBody                                  13
///�����
#define DFITCSEC_AI_Foundation                               14
///̨��֤
#define DFITCSEC_AI_TaiWanCard                               15
///�۰�̨�������֤
#define DFITCSEC_AI_HMTIDcard                                16
///����
#define DFITCSEC_AI_OtherCard                                99

/////////////////////////////////////////////////////////////////////////
///DFITCSBranchTypeType ��һ��������־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSBranchTypeType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECPasswdSyncFlagType ��һ������ͬ����־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECPasswdSyncFlagType;
///��ͬ��
#define DFITCSEC_PSF_Sync                                     0
///ͬ��
#define DFITCSEC_PSF_UnSync                                   1

/////////////////////////////////////////////////////////////////////////
///DFITCSECShareholderSpecPropType ��һ���ɶ�������������
/////////////////////////////////////////////////////////////////////////
typedef int   DFITCSECShareholderSpecPropType;
///����ָ��
#define DFITCSEC_SSP_SpecTheDay                               2
///�ع�ָ��
#define DFITCSEC_SSP_SpecRePurchase                           16
///����ָ��
#define DFITCSEC_SSP_SpecOtherDay                             32

/////////////////////////////////////////////////////////////////////////
///DFITCSECTradePermissionsType ��һ������Ȩ������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECTradePermissionsType;
///��������Ȩ��
#define DFITCSEC_TP_BasicTrade                                 1
///�ع�����
#define DFITCSEC_TP_ReprchFnc                                  2
///�ع���ȯ
#define DFITCSEC_TP_ReprchSecLoan                              4
///ETF����
#define DFITCSEC_TP_ETF_SubRedemp                              8
///Ȩ֤����
#define DFITCSEC_TP_WarrantTrade                               16
///���屨��ת��
#define DFITCSEC_TP_ThreeBrdTran                               32
///��ҵ��
#define DFITCSEC_TP_GEM                                        64

/////////////////////////////////////////////////////////////////////////
///DFITCSECShareholderStatusType ��һ���ɶ�״̬����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECShareholderStatusType;
///����
#define DFITCSEC_SHS_Normal                                    0
///�ɶ�����
#define DFITCSEC_SHS_Freeze                                    1
///�ɶ�����ʧ
#define DFITCSEC_SHS_LOSS                                      2
///С������
#define DFITCSEC_SHS_Dormancy                                  6
///���ϸ�
#define DFITCSEC_SHS_Unqualified                               7
///����
#define DFITCSEC_SHS_Activation                                8
///��˾���ϸ�
#define DFITCSEC_SHS_CompanyUnqualified                        9
///ʹ��������
#define DFITCSEC_SHS_Applying                                  10
///ʹ������ʧ��
#define DFITCSEC_SHS_ApplyFailed                               11
///VIP״̬
#define DFITCSEC_SHS_VIP                                       12

/////////////////////////////////////////////////////////////////////////
///DFITCSECMainAccountFlagType ��һ�����˻���־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECMainAccountFlagType;
///�����ʺ�
#define DFITCSEC_MA_Auxiliary                                  0
///���ʺ�
#define DFITCSEC_MA_Master                                     1

/////////////////////////////////////////////////////////////////////////
///DFITCSECShareholderCtlPropType ��һ���ɶ�������������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECShareholderCtlPropType;
///����
#define DFITCSEC_SHC_Normal                                    0
///��ֹ����
#define DFITCSEC_SHC_ForbidBuy                                 4
///��ֹ����
#define DFITCSEC_SHC_ForbidSell                                8
///��ֹ��ָ
#define DFITCSEC_SHC_ForbidWithdraw                            64
///��ֹת�й�
#define DFITCSEC_SHC_ForbidCustodyTransfer                     128

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderRangeID ��һ��ί�кŷ�Χ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECOrderRangeID[1024];

/////////////////////////////////////////////////////////////////////////
///DFITCSECBidTradeFlagType ��һ�����۽��ױ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECBidTradeFlagType;
///�������Ͼ����ڼ佻�� 
#define DFITCSEC_BTF_UnBidTrade                           0
///�����Ͼ����ڼ佻��
#define DFITCSEC_BTF_BidTrade                             1

/////////////////////////////////////////////////////////////////////////
///DFITCSECTradeUnitType ��һ�����׵�λ����
/////////////////////////////////////////////////////////////////////////
typedef int   DFITCSECTradeUnitType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECBusinessLimitType ��һ��������������
/////////////////////////////////////////////////////////////////////////
typedef int   DFITCSECBusinessLimitType;
///T+0����
#define DFITCSEC_T0_Transaction                          1
///ϯλ�й�
#define DFITCSEC_Seat_Hosting                            2
///������ֵ
#define DFITCSEC_Computing_Market_Value                  8

/////////////////////////////////////////////////////////////////////////
///DFITCSECSubAccountIDType �����˻���������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECSubAccountIDType[7];

/////////////////////////////////////////////////////////////////////////
///DFITCSECOpenCloseFlagType ��һ����ƽ��־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOpenCloseFlagType;
///����
#define DFITCSEC_OCF_Open                                1
///ƽ��
#define DFITCSEC_OCF_Close                               2
///ִ��
#define DFITCSEC_OCF_Execute                             6
///��Լ
#define DFITCSEC_OCF_Perform                             7
///ʵ�����򽻸�
#define DFITCSEC_OCF_RealIntenDevi                       8
///��Լ�ֲֵ���                     
#define DFITCSEC_OCF_ConPosiAdj                          9 
///֤ȯ����
#define DFITCSEC_OCF_SecFreeze                           10
///֤ȯ�ⶳ
#define DFITCSEC_OCF_SecThaw                             11
///ǿƽ
#define DFITCSEC_OCF_ForceClose                          12
///���
#define DFITCSEC_OCF_Group                               19
///���
#define DFITCSEC_OCF_Split                               20
///ת����
#define DFITCSEC_OCF_ToPrePare                           21

/////////////////////////////////////////////////////////////////////////
///DFITCSECCoveredFlagType ��һ�����ұ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECCoveredFlagType;
///�Ǳ���
#define DFITCSEC_CF_UnCovered                            0
///����
#define DFITCSEC_CF_Covered                              1
///��������(Ԥ����Ŀǰ��֧��)
#define DFITCSEC_CF_CoveredPrior                         2

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderExpiryDateType ��һ������ʱЧ��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOrderExpiryDateType;
///������Ч
#define DFITCSEC_OE_NONE                                 0
///�����ɽ�ʣ��ָ���Զ�����ָ�� FAK(IOC)
#define DFITCSEC_OE_FAK                                  1
///����ȫ���ɽ������Զ�����ָ�� FOK
#define DFITCSEC_OE_FOK                                  2

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderCategoryType ��һ��ί�е��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOrderCategoryType;
///��ͨί��
#define DFITCSEC_OC_GeneralOrder                         0
///�ֶ�ǿƽ��
#define DFITCSEC_OC_ManualCloseOrder                     1
///���鴥����(Ԥ����Ŀǰϵͳ��֧��)
//#define DFITCSEC_OC_QuoteTriggOrder                      2
///�Զ�ǿƽ�� 
#define DFITCSEC_OC_AutoCloseOrder                       8

/////////////////////////////////////////////////////////////////////////
///DFITCSECTDevIDType ��һ�������������̴�������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECTDevIDType[13];

/////////////////////////////////////////////////////////////////////////
///DFITCSECTDevDecInfoType ��һ���������������걨��Ϣ����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECTDevDecInfoType[101];

/////////////////////////////////////////////////////////////////////////
///DFITCSECOptionTypeType ��һ����Ȩ�������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOptionTypeType;
///�Ϲ���Ȩ
#define DFITCSEC_OT_CALL                                1
///�Ϲ���Ȩ
#define DFITCSEC_OT_PUT                                 2

/////////////////////////////////////////////////////////////////////////
///DFITCSECContractObjectTypeType ��һ�������������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECContractObjectTypeType;
///A��
#define DFITCSEC_COT_STOCK                              1
///ETF
#define DFITCSEC_COT_ETF                                2

/////////////////////////////////////////////////////////////////////////
///DFITCSECContractUnitType ��һ����Ȩ��Լ��λ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECContractUnitType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECContractIDType ��һ����Ȩ��Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECContractIDType[20];


/////////////////////////////////////////////////////////////////////////
///DFITCSECContractIDType ��һ����Ʒʵʱ�׶μ���־����
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECTradingPhaseCodeType[9];

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountTypeType ��һ��Ͷ�����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountTypeType;
///����
#define DFITCSEC_AT_Personal                           1
///����
#define DFITCSEC_AT_Organization                       2
///��Ӫ
#define DFITCSEC_AT_Selfoperation                      3
///������
#define DFITCSEC_AT_Marketmaker                        4
///���׻�Ա
#define DFITCSEC_AT_member                             0

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountPropType ��һ���ͻ���������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountPropType;
///��ֹ���
#define DFITCSEC_AP_PrhbtDeposit                       1
///��ֹȡ��
#define DFITCSEC_AP_PrhbtDrawMoney                     2
///��ֹ��֤ת��
#define DFITCSEC_AP_PrhbtBankTran                      4
///��ֹת�й�
#define DFITCSEC_AP_PrhbtTransfer                      8
///��ֹ��ָ
#define DFITCSEC_AP_PrhbtCancelSpecTran                16
///��ֹ����
#define DFITCSEC_AP_PrhbtCancelAcc                     32
///��ֹ����
#define DFITCSEC_AP_PrhbtTakePos                       64
///��ֹƽ��
#define DFITCSEC_AP_PrhbtClsPos                        128
///��ֹ�����ʽ���ת
#define DFITCSEC_AP_PrhbtSngFamFunTran                 256

/////////////////////////////////////////////////////////////////////////
///DFITCSECAccountNodeIDType ��һ���ͻ��ڵ�������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECAccountNodeIDType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECCheckUpLimitFlagType ��һ�����ί�����ޱ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECCheckUpLimitFlagType;
///���
#define DFITCSEC_CULF_Check                           0
///�����
#define DFITCSEC_CULF_UnChec                          1

/////////////////////////////////////////////////////////////////////////
///DFITCSECContractNameType ��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECContractNameType[32];

/////////////////////////////////////////////////////////////////////////
///DFITCSECExecuteTypeType ��һ����Ȩ��ʽ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECExecuteTypeType;
///��ʽ
#define DFITCSEC_ET_US                                0
///ŷʽ
#define DFITCSEC_ET_Europe                            1

/////////////////////////////////////////////////////////////////////////
///DFITCSECDeliveryTypeType ��һ�����ʽ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECDeliveryTypeType;
///ʵ�ｻ��
#define DFITCSEC_DT_Real                              1
///�ֽ𽻸�
#define DFITCSEC_DT_Cash                              2

/////////////////////////////////////////////////////////////////////////
///DFITCSECOpenLimitsType ��һ��������������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOpenLimitsType;
///������
#define DFITCSEC_OL_Permit                            0
///���ƿ���
#define DFITCSEC_OL_UnPermit                          1

/////////////////////////////////////////////////////////////////////////
///DFITCSECStockTradeFlagType ��һ����Ʊ���ױ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECStockTradeFlagType;
///Ϊ"ͣ�Ʊ�־"ʱ, ȡֵ����
///����
#define DFITCSEC_STF_Normal                           0
///ͣ��
#define DFITCSEC_STF_Stop                             1
///Ϊ"����״̬"ʱ, ȡֵ����
///����
#define DFITCSEC_STF_Normal                           0
///ͣ��
#define DFITCSEC_STF_Stop                             1
///����
#define DFITCSEC_STF_Issue                            2
///��������
#define DFITCSEC_STF_FirstList                        3
///����
#define DFITCSEC_STF_UnList                           4

////////////////////////////////////////////////////////////////////////
///DFITCSECApproachExpireFlagType ��һ���ٽ������ձ�־����
////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECApproachExpireFlagType;

////////////////////////////////////////////////////////////////////////
///DFITCSECTempAdjuestFlagType ��һ��������־����
////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECTempAdjuestFlagType;

////////////////////////////////////////////////////////////////////////
///DFITCSECStockListFlagType ��һ�����Ʊ�־����
////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECStockListFlagType[2];
///�����¹��ƺ�Լ
#define DFITCSEC_SLF_A                               'A'
///�����ĺ�Լ
#define DFITCSEC_SLF_E                               'E'
///����ժ�Ƶĺ�Լ
#define DFITCSEC_SLF_D                               'D'

////////////////////////////////////////////////////////////////////////
///DFITCSECContractObjectStatusType ��һ�����֤ȯ״̬����
////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECContractObjectStatusType;
///����
#define DFITCSEC_COS_Normal                          0
///��ͣ
#define DFITCSEC_COS_Suspend                         1
///����
#define DFITCSEC_COS_Cancel                          2

/////////////////////////////////////////////////////////////////////////
///DFITCSECExchangeNameType ��һ����������������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECExchangeNameType[11];

/////////////////////////////////////////////////////////////////////////
///DFITCSECNightTradingFlagType ��һ��ҹ�н��ױ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECNightTradingFlagType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECTradingDayFlagType ��һ�������ձ�־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECTradingDayFlagType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECLvelCategoryType ��һ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECLvelType;
///������
#define DFITCSEC_L_EXCHANGE                         1
///��˾
#define DFITCSEC_L_COMPANY                          2
///����
#define DFITCSEC_L_CUSTOMER                         3

/////////////////////////////////////////////////////////////////////////
///DFITCSECReferenceTypeType ��һ�����ʲ����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECReferenceTypeType;
///���������
#define DFITCSEC_RT_ObjectType                      1
///����Ĵ���
#define DFITCSEC_RT_ObjectID                        2
///����Ȩ����
#define DFITCSEC_RT_OptionID                        3

/////////////////////////////////////////////////////////////////////////
///DFITCSECCalcTypeType ��һ�����㷽ʽ����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECCalcTypeType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECContractNOType ��һ��ָ����Լ�������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECContractNOType[33];

/////////////////////////////////////////////////////////////////////////
///DFITCSECCrdtContractQryFlagType ��һ����ѯ��־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECCrdtContractQryFlagType;
///��ѯ����
#define DFITCSEC_CCQF_Queryall                      0
///�����˽�
#define DFITCSEC_CCQF_Endoftheday                   1
///δ�˽�
#define DFITCSEC_CCQF_Unfinished                    2

/////////////////////////////////////////////////////////////////////////
///DFITCSECCrdtContractStatusType ��һ����Լ״̬����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECCrdtContractStatusType;
///δ�˽�
#define DFITCSEC_CCS_UnSettle                      0
///���˽�δ����
#define DFITCSEC_CCS_UnClear                       1
///���˽�
#define DFITCSEC_CCS_Settle                        3

/////////////////////////////////////////////////////////////////////////
///DFITCSECPositionNOType ��һ��ͷ��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECPositionNOType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECClearFlagType ��һ�������־����
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECClearFlagType;
///�ɽ�����
#define DFITCSEC_CF_TradeCause                     1
///��������
#define DFITCSEC_CF_ClearCause                     2
///Ȩ�油������
#define DFITCSEC_CF_ComCause                       3

/////////////////////////////////////////////////////////////////////////
///DFITCSECShareholderTypeType ��һ���ɶ��������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECShareholderTypeType;

/////////////////////////////////////////////////////////////////////////
///DFITCSECHKEntrustLimitType ��һ���۹ɽ�����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECHKEntrustLimitType;
///��ͨ��Ʊ
#define DFITCSEC_HKEL_NormalStolk                  0
///����
#define DFITCSEC_HKEL_Bid                          1
///���о���
#define DFITCSEC_HKEL_OpeningBid                   2
///���               
#define DFITCSEC_HKEL_OddStock                     3

/////////////////////////////////////////////////////////////////////////
///DFITCSECHKOrderTypeType ��һ���۹ɶ�����������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECHKOrderTypeType;
///������Ч
#define DFITCSEC_HKOT_GFT                          0
///ȫ�ʱ
#define DFITCSEC_HKOT_FOK                          4

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderConfirmFlagType ��һ��ί��ȷ������
/////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECOrderConfirmFlagType;
///ί�гɹ�
#define DFITCSEC_ORDER_SUCCESS                     0
///ί��ʧ��
#define DFITCSEC_ORDER_FAIL                        1

//////////////////////////////////////////////////////////////////////////
///DFITCSECExchangeGroupTypeType ��һ���������������
//////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECExchangeGroupTypeType;
///ת����
#define DFITCSEC_EGT_ToPrepare                     1
///�Ϲ�ţ�м۲����
#define DFITCSEC_EGT_BullBuy                       2
///�Ϲ����м۲����
#define DFITCSEC_EGT_BearSell                      3
///�Ϲ�ţ�м۲����
#define DFITCSEC_EGT_BullSell                      4
///�Ϲ����м۲����
#define DFITCSEC_EGT_BearBuy                       5
///��ʽ��ͷ
#define DFITCSEC_EGT_ShortStraddle                 6
///���ʽ��ͷ
#define DFITCSEC_EGT_WideStraddle                  7
///����ת�ֽ�
#define DFITCSEC_EGT_CoveredToCash                 8

//////////////////////////////////////////////////////////////////////////
///DFITCSECGroupCodeType ��һ����ϱ�������
//////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECGroupCodeType[21];

//////////////////////////////////////////////////////////////////////////
///DFITCSECContractAdjustRemindType ��һ����Լ������������
//////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECContractAdjustRemindType;
///δ����
#define DFITCSEC_CAR_NotAdj                        0
///�ѵ���
#define DFITCSEC_CAR_Adj                           1

//////////////////////////////////////////////////////////////////////////
///DFITCSECContraceExpireRemindType ��һ����Լ����������������
//////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECContraceExpireRemindType;
///δ����
#define DFITCSEC_CER_NotExpire                     0
///���뵽���ղ���10��������
#define DFITCSEC_CER_WillExpire                    1

//////////////////////////////////////////////////////////////////////////
///DFITCSESecurityOptionIndexType ��һ����Ʊ��Ȩ����ֵ����
//////////////////////////////////////////////////////////////////////////
typedef char   DFITCSESecurityOptionIndexType[21];

/////////////////////////////////////////////////////////////////////////
///DFITCSECOrderTypeLimitType ��һ������������������
/////////////////////////////////////////////////////////////////////////
typedef char   DFITCSECOrderTypeLimitType[128];

/////////////////////////////////////////////////////////////////////////
///DFITCSECMiniPriceChangeType ��һ����С�䶯��λ����
/////////////////////////////////////////////////////////////////////////
typedef double DFITCSECMiniPriceChangeType;



//////////////////////////////////////////////////////////////////////////
///DFITCSECMDCompressFalgType ����ѹ����־����
//////////////////////////////////////////////////////////////////////////
typedef int    DFITCSECMDCompressFalgType;
///��Ҫ������ǰ��ѹ������
#define DFITCSEC_COMPRESS_FALSE                                      0
///Ҫ������ǰ��ѹ������
#define DFITCSEC_COMPRESS_TRUE                                       1

#endif//DFITCStockAPIDATATYPE_H
