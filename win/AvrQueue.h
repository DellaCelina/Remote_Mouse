#pragma once

#define LEN 40

class AvrQueue {
private:
	int arr[LEN];
	int len;
	int ip;	//insert point
public:
	AvrQueue():len(0), ip(0){
		int i;
		for (i = 0; i < LEN; i++)	arr[i] = 0;
	}
	~AvrQueue() {};
	void InitQueue() {
		int i;
		for (i = 0; i < LEN; i++)	arr[i] = 0;
		ip = 0;
		len = 0;
	}
	void Push(int value) {
		arr[ip] = value;
		if (len != LEN) len++;	//len�� ����� LEN���� ����
		if (++ip == LEN) ip = 0;	//ip�� �迭�� ������ ���� �� 0���� �ǵ��ƿ�
	}
	int Avr() {
		int i, result = 0;
		for (i = 0; i < len; i++) result += arr[i];
		return (int)(result / len);
	}
};
