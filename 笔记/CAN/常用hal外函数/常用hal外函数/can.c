#include<stdio.h>
// ��ʼ��CAN����
void CAN_Init(CAN_HandleTypeDef* hcan) {
	HAL_CAN_Start(hcan);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}
// ��������֡
uint8_t CAN_Send_Data(CAN_HandleTypeDef* hcan, uint16_t ID, uint8_t* Data, uint8_t Length) {
	CAN_TxHeaderTypeDef tx_header;
	uint32_t used_mailbox;
	// ���ؼ�����
	assert_param(hcan != NULL);

	tx_header.StdId = ID;
	tx_header.ExtId = 0;
	tx_header.IDE = 0;
	tx_header.RTR = 0;
	tx_header.DLC = Length;

	return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}
// �˲���
void CAN_Filter_Mask_Config(CAN_HandleTypeDef* hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID) {
	CAN_FilterTypeDef can_filter_init_structure;
	assert_param(hcan != NULL);
	if ((Object_Para & 0x02)) {
		// ����֡
		// �����ID�ĸ�16λbit
		can_filter_init_structure.FilterIdHigh = ID << 3 << 16;
		//�����ID�ĵ�16λbit
		can_filter_init_structure.FilterIdLow = ID << 3 | ((Object_Para & 0x03) << 1);
		// ID����ֵ��16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 3 << 16;
		// ID����ֵ��16bit
		can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | ((Object_Para & 0x03) << 1);
	}
	else {
		// ң��֡
		//�����ID�ĸ�16λbit
		can_filter_init_structure.FilterIdHigh = ID << 5;
		//�����ID�ĵ�16λbit
		can_filter_init_structure.FilterIdLow = ((Object_Para & 0x03) << 1);
		// ID����ֵ��16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 5;
		// ID����ֵ��16bit
		can_filter_init_structure.FilterMaskIdLow = ((Object_Para & 0x03) << 1);
	}
	// �˲������,0-27,��28���˲���,can1��0~13,can2��14~27,c8t6����14���˲���
	can_filter_init_structure.FilterBank = (Object_Para >> 3) & 0x0F;
	// �˲�����FIFOx,ֻ�ܰ�һ��
	can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
	// ʹ���˲���
	can_filter_init_structure.FilterActivation = ENABLE;
	// �˲���ģʽ,����ID����ģʽ
	can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
	// 32λ�˲�
	can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
	// �ӻ�ģʽѡ��ʼ��Ԫ
	can_filter_init_structure.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure);
}