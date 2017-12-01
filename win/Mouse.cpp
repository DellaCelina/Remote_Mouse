#include "Serial.h"
#include "mouse.h"
#include "AvrQueue.h"

#define PORT_LEN 10

#define SENSOR_MIN 260
#define SENSOR_MAX 400
#define SENSOR_SUB (SENSOR_MIN + (SENSOR_MAX-SENSOR_MIN) / 2)

#define PI 3.1415926

#define CONST_FOR_SENSE 100

void mouseStart(Serial& port);
inline int checkSign(UINT16);

int main() {
	try {
		LPCWSTR ptname = new WCHAR[PORT_LEN];
		printf("Select port: ");
		wscanf(L"%s", ptname);
		Serial port(ptname, CBR_9600, 8, ONESTOPBIT, NOPARITY);
		wprintf(L"%s Connect Success\n", ptname);

		printf("Set Mouse Sensitivity: ");
		scanf("%f", &mouse_sensitivity);
		mouseStart(port);
	}
	catch (Exception e) {
		e.print();
	}
	return 0;
}

void mouseStart(Serial& port) {
	char uartBuf[UARTSIZE];
	POINT mousePt, mouseMPt, curruntPt = { 0 }, pastPt = { 0 };	//mousePt : ���� Ŀ��, mouseMPt : ������ ��ȭ��, curruntPt : ���콺 ������ ���۽��� ��ġ, pastPt: ��ȭ�� ����� ���� ���� ����Ʈ
	UINT16 mouseMove = 0;	//uart�� �޴� ������
	long convd;		//mouseMove�� ��ȣ�� �ִ� ��ȭ������ ��ȯ�� ������
	AvrQueue xqu = AvrQueue();	//x�������� ����� ���ִ� ť
	AvrQueue yqu = AvrQueue();	//y�������� ����� ���ִ� ť

	while (1) {
		if(!port.Read(uartBuf, 2)) throw Exception("Disconnected Port\n", HANDLEERR);
		GetCursorPos(&mousePt);

		//upper bit�� ���� 3��Ʈ�� event üũ
		switch (uartBuf[0] & 0xE0) {
		case MOUSELEFTDOWN:
			mouse_event(MOUSEEVENTF_LEFTDOWN, mousePt.x, mousePt.y, 0, 0);
			printf("leftdown\n");
			break;
		case MOUSELEFTUP:
			mouse_event(MOUSEEVENTF_LEFTUP, mousePt.x, mousePt.y, 0, 0);
			printf("leftup\n");
			break;
		case MOUSERIGHTDOWN:
			mouse_event(MOUSEEVENTF_RIGHTDOWN, mousePt.x, mousePt.y, 0, 0);
			printf("rightdown\n");
			break;
		case MOUSERIGHTUP:
			mouse_event(MOUSEEVENTF_RIGHTUP, mousePt.x, mousePt.y, 0, 0);
			printf("rightup\n");
			break;
		case MOUSEMOVEX:
			mouseMove = ((uartBuf[0] & 0x1F) << 8) + (uartBuf[1] & 0xFF);
			convd = checkSign(mouseMove);
			printf("moveX: %d\n", convd);
			xqu.Push(convd);
			mouseMPt.x = xqu.Avr() * mouse_sensitivity / CONST_FOR_SENSE;
			break;
		case MOUSEMOVEY:
			mouseMove = ((uartBuf[0] & 0x1F) << 8) + (uartBuf[1] & 0xFF);
			convd = checkSign(mouseMove);
			printf("moveY: %d\n\n", convd);
			yqu.Push(convd);
			mouseMPt.y = yqu.Avr() * mouse_sensitivity / CONST_FOR_SENSE;
			SetCursorPos(curruntPt.x + mouseMPt.x, curruntPt.y + mouseMPt.y);
			break;
		case MOUSESTART:
			mouseMove = ((uartBuf[0] & 0x1F) << 8) + (uartBuf[1] & 0xFF);
			GetCursorPos(&curruntPt);
			pastPt.x = curruntPt.x, pastPt.y = curruntPt.y;
			xqu.InitQueue();
			yqu.InitQueue();
			printf("mouseStart\n");
		}
	}
}

inline int checkSign(UINT16 ck) {	//��ȣ �����͸� ��ȯ
	return (ck & SIGN) ? -(int)(ck - SIGN) : (int)ck;
}