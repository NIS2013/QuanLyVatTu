// Các tính năng còn thiếu
// - kiểm tra điều kiện dữ liệu nhập vào
// - các tính năng

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <iomanip>

#include "mylib.h"

using namespace std;

// Hằng số
const int SO_ITEM_MENU = 16;

const int MAX_VAT_TU = 100;
const int MAX_CTHD = 20;

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;

char listThucDon[SO_ITEM_MENU][50] = {  "--1.  Them vat tu",
										"--2.  Xoa vat tu",
										"--3.  Hieu Chinh Vat Tu",
										"x-4.  Liet ke danh sach vat tu",
										"--5.  Them nhan vien",
										"--6.  Xoa nhan vien",
										"--7.  Hieu chinh nhan vien",
										"x-8.  In danh sach nhan vien", // chưa liệt kê theo thứ tự yêu cầu
										"x-9.  Lap hoa don",
										"10. Tra hang",
										"11. In hoa don",
										"12. Liet ke hoa don theo nhan vien",
										"13. Thong ke doanh thu theo nam",
										"14. Load du lieu tu file",
										"15. Luu du lieu vao file",
										"--16. Thoat" };

// Biến
int dong = 5;
int cot = 10;

// Danh sách thực đơn

void ThongBao(const char* s) { // Hàm thông báo lỗi
	int x = wherex(), y = wherey();
	gotoxy(10, 24);
	cout << s;
	_getch();
	gotoxy(10, 24);
	clreol();
	gotoxy(x, y);
}

// ------------- Định nghĩa các struct -------------- //

// ++++ Ngày ++++++ //
struct Ngay {
	int ngay;
	int thang;
	int nam;
};

// +++ End ngày +++ //

// +++ Vật tư +++++ //
struct VatTu {
	string maVatTu;
	string tenVatTu;
	string donViTinh;
	float soLuongTon;
};

struct ListVT {
	int n;
	VatTu* list[MAX_VAT_TU];
};

// ++ End vật tư ++ //

// ++ Chi tiết hoá đơn ++ //
struct ChiTietHoaDon {
	string maVatTu;
	int soLuong;
	int donGia;
	float VAT;
	bool trangThai;
};

struct ListCTHD {
	int n;
	ChiTietHoaDon list[MAX_CTHD];
}; // danh sách tuyến tính CTHD

// +++++ CTHD +++++++ //

// ++++ Hoá đơn +++++ //
struct HoaDon {
	string soHoaDon;
	Ngay ngayLap;
	char loai;
	ListCTHD listCTHD;
};

// Danh sách liên kết đơn (Hoá đơn)
struct LListHDNode {
	HoaDon data;
	struct LListHDNode* pNext;
};

typedef LListHDNode* LListHD;

LListHDNode* TaoNodeLListHD(HoaDon hoaDon) { // Hàm tạo 1 node cho DSLKĐ
	LListHDNode* node = new LListHDNode;
	if (node == NULL) // khởi tạo con trỏ bị lỗi
		return NULL;
	node->data = hoaDon;
	node->pNext = NULL;
	return node;
}

void ThemNodeLListHD(LListHD & L, LListHDNode * node) // Thêm node vào đầu danh sách
{
	node->pNext = L;
	L = node;
   }


LListHDNode* TimNodeLListHD(LListHD L, string maHoaDon) { // Tìm node trong DS
	while (L != NULL)
		if (L->data.soHoaDon == maHoaDon)
			return L;
		else
			L = L->pNext;
	return NULL;
}

// +++ End hoá đơn +++++ //

// +++ Nhân viên ++ //
struct NhanVien {
	int maNhanVien;
	string ho;
	string ten;
	LListHD listHD;
};

// BST Nhân viên
struct BSTNVNode {
	int key;
	NhanVien data;
	BSTNVNode* pLeft;
	BSTNVNode* pRight;
};
typedef BSTNVNode* BSTNVRoot;

BSTNVNode* TaoNodeBSTNV(NhanVien data) {
	BSTNVNode* node = new BSTNVNode;
	if (node == NULL) // khởi tạo con trỏ bị lỗi
		return NULL;
	node->key = data.maNhanVien;
	node->data = data;
	node->pLeft = NULL;
	node->pRight = NULL;
	return node;
};

void ThemNodeBSTNV(BSTNVRoot & L, BSTNVNode * node) {
	if (L == NULL) // TH cây rỗng
		L = node;
	else {
		// Tìm node cha của node cần thêm vào
		BSTNVNode* nodeDuyet = L;
		BSTNVNode* nodeCha = NULL;
		while (nodeDuyet != NULL) {
			nodeCha = nodeDuyet;
			if (node->key < nodeDuyet->key)
				nodeDuyet = nodeDuyet->pLeft;
			else if (node->key > nodeDuyet->key)
				nodeDuyet = nodeDuyet->pRight;
		}
		// thêm node con vào node cha đã tìm được
		if (node->key < nodeCha->key)
			nodeCha->pLeft = node;
		else if (node->key > nodeCha->key)
			nodeCha->pRight = node;
		else
			ThongBao("Key da ton tai");
	}
};

BSTNVNode* TimNodeBSTNV(BSTNVRoot & L, int key) { // Tim theo ma nv
	if (L == NULL) // cay rong
		return NULL;
	if (L->key == key)
		return L;
	else if (key < L->key)
		return TimNodeBSTNV(L->pLeft, key);
	else if (key > L->key)
		return TimNodeBSTNV(L->pRight, key);
}

void XoaNodeBSTNV(BSTNVRoot & L, int key) {
	if (L == NULL) {
		ThongBao("Cay rong");
		return;
	}

	bool timThayNode = false;
	BSTNVNode* nodeDuyet = L;
	BSTNVNode* nodeCha = NULL;

	while (nodeDuyet != NULL) {
		if (nodeDuyet->key == key) {
			timThayNode = true;
			break;
		}
		else {
			nodeCha = nodeDuyet;
			if (key < nodeDuyet->key)
				nodeDuyet = nodeDuyet->pLeft;
			else
				nodeDuyet = nodeDuyet->pRight;
		}
	}
	if (!timThayNode) {
		ThongBao("Key khong ton tai");
		return;
	}

	// TH node không có node con
	if (nodeDuyet->pLeft == NULL && nodeDuyet->pRight == NULL) {
		if (nodeCha->pLeft == nodeDuyet)
			nodeCha->pLeft = NULL;
		else
			nodeCha->pRight = NULL;
		delete nodeDuyet;
		return;
	}

	// TH node xoá có 1 node con
	if (nodeDuyet->pLeft == NULL || nodeDuyet->pRight == NULL) {
		if (nodeDuyet->pLeft == NULL) {
			if (nodeCha->pLeft == nodeDuyet)
				nodeCha->pLeft = nodeDuyet->pRight;
			else
				nodeCha->pRight = nodeDuyet->pRight;
		}
		else {
			if (nodeCha->pLeft == nodeDuyet)
				nodeCha->pLeft = nodeDuyet->pLeft;
			else
				nodeCha->pRight = nodeDuyet->pLeft;
		}
		delete nodeDuyet;
		return;
	}

	// TH node xoá có đủ 2 node con
	if (nodeDuyet->pLeft != NULL && nodeDuyet->pRight != NULL) {
		BSTNVNode* tmp = nodeDuyet->pRight;
		if (tmp->pLeft == NULL) { // Node xoá có node con phải không có node trái
			nodeDuyet->key = tmp->key;
			nodeDuyet->data = tmp->data;
			nodeDuyet->pRight = tmp->pRight;
			delete tmp;
		}
		else { // Node xoá có node con phải có node trái -> duyệt đến node trái nhỏ nhất
			BSTNVNode* nodeDuyet2 = tmp;
			BSTNVNode* nodeCha2 = tmp->pLeft;
			while (nodeDuyet2->pLeft != NULL) {
				nodeCha2 = nodeDuyet2;
				nodeDuyet2 = nodeDuyet2->pLeft;
			}
			nodeDuyet->key = nodeDuyet2->key;
			nodeDuyet->data = nodeDuyet2->data;
			nodeCha2->pLeft = nodeDuyet2->pRight;
			delete nodeDuyet2;
		}
	}
}

int SoPhanTuBSTNV(BSTNVRoot L) {
	if (L == NULL)
		return 0;
	if (L->pLeft == NULL && L->pRight == NULL)
		return 1;
	return 1 + SoPhanTuBSTNV(L->pLeft) + SoPhanTuBSTNV(L->pRight);
}

void LayMangBSTNV(BSTNVRoot L, NhanVien* mangNV, int &n ) {
	if (L != NULL) {
		mangNV[n] = L->data;
		n++;
		LayMangBSTNV(L->pLeft, mangNV, n);
		LayMangBSTNV(L->pRight, mangNV, n);
	}
}

LListHDNode* TimNodeHDBSTNV(BSTNVRoot L, string key) { // duyet node - left - right
	if (L == NULL)
		return NULL;

	LListHDNode * nodeTrai = NULL;
	LListHDNode * nodePhai = NULL;
	LListHDNode * nodeHD = L->data.listHD;

	while (nodeHD != NULL)
		if (nodeHD->data.soHoaDon == key)
			return nodeHD;
		else nodeHD = nodeHD->pNext;

	nodeTrai = TimNodeHDBSTNV(L->pLeft, key);
	nodePhai = TimNodeHDBSTNV(L->pRight, key);
	if (nodeTrai != NULL)
		return nodeTrai;
	if (nodePhai != NULL)
		return nodePhai;
	return NULL;
}

void Normal() {
	SetColor(15);
	SetBGColor(0);
}
void HighLight() {
	SetColor(15);
	SetBGColor(1);
}

void VeMenuChinh(char listThucDon[SO_ITEM_MENU][50], int itemDaChon) { // Hàm hiển thị các menu chính
	Normal();
	for (int i = 0; i < SO_ITEM_MENU; i++) {
		gotoxy(cot, dong + i);
		cout << listThucDon[i];
	}
	HighLight();
	gotoxy(cot, dong + itemDaChon);
	cout << listThucDon[itemDaChon];
	Normal();
}

int ChonMenuChinh(char listThucDon[SO_ITEM_MENU][50]) { // Hàm chọn các menu chính
	clrscr();
	int itemDaChon = 0;
	VeMenuChinh(listThucDon, itemDaChon);
	char phim;
	for (;;) {
		phim = _getch();
		switch (phim)
		{
		case KEY_UP:
			if (itemDaChon + 1 > 1)
				itemDaChon--;
			else
				itemDaChon = SO_ITEM_MENU - 1;
			VeMenuChinh(listThucDon, itemDaChon);
			break;
		case KEY_DOWN:
			if (itemDaChon + 1 < SO_ITEM_MENU)
				itemDaChon++;
			else
				itemDaChon = 0;
			VeMenuChinh(listThucDon, itemDaChon);
			break;
		case KEY_ENTER:
			return itemDaChon + 1;
		}
	}
}

int TimVatTuTheoMa(ListVT listVT, string maVatTu) { // Tìm vị trí vật tư theo mã vật tư
	for (int i = 0; i < listVT.n; i++)
		if (listVT.list[i]->maVatTu == maVatTu)
			return i;
	return -1;
}

void NhapVatTu(ListVT & listVT) { // Nhap danh sach vat tu
	VatTu vatTu;
	for (;;) {
		// TH danh sách đầy
		if (listVT.n > MAX_VAT_TU)
		{
			ThongBao("Danh sach day");
			return;
		}

		clrscr(); // Xoa man hinh
		cout << "So vat tu da nhap: " << listVT.n << endl;
		// Nhap du lieu
		cout << "Nhap ma vat tu (Nhap X de ket thuc): ";
		getline(cin, vatTu.maVatTu);

		// Thoat neu nhap x
		if (vatTu.maVatTu == "X" || vatTu.maVatTu == "x")
			return;

		// Kiểm tra chiều dài mã vật tư
		if (vatTu.maVatTu.length() > 10) {
			ThongBao("Ma vat tu co toi da 10 ki tu");
			continue;
		}

		// Kiem tra trung ma vat tu
		if (TimVatTuTheoMa(listVT, vatTu.maVatTu) != -1) {
			ThongBao("Trung ma vat tu");
			continue;
		}

		cout << "Nhap ten vat tu: ";
		getline(cin, vatTu.tenVatTu);
		cout << "Don vi tinh: ";
		getline(cin, vatTu.donViTinh);
		cout << "So luong ton: ";

		// Xu ly nhap so luong ton khac float
		cin >> vatTu.soLuongTon;
		while (cin.fail()) {
			ThongBao("Vui long chi nhap so thuc.");
			cin >> vatTu.soLuongTon;
		}
		cin.ignore();

		// Them vao danh sach
		listVT.list[listVT.n] = new VatTu(vatTu);
		listVT.n++;
	};
}

void XoaVatTu(ListVT & listVT) {
	string maVatTu;
	for (;;) {
		clrscr(); // Xoa man hinh
		cout << "Nhap ma vat tu can xoa (Nhap X de thoat): ";
		getline(cin, maVatTu);

		// Kiểm tra chiều dài mã vật tư
		if (maVatTu.length() != 10) {
			ThongBao("Ma vat tu phai co 10 ki tu");
			continue;
		}

		if (maVatTu == "X" || maVatTu == "x")
			return;
		int viTri = TimVatTuTheoMa(listVT, maVatTu);
		if (viTri == -1) // Tim thay
			ThongBao("Khong tim thay ma vat tu");
		else { // Khong tim thay
			delete listVT.list[viTri];
			for (int i = viTri; i < listVT.n - 1; i++)
				listVT.list[i] = listVT.list[i + 1];
			listVT.n--;
			ThongBao("Da xoa vat tu");
		}
	}
}

void HieuChinhVatTu(ListVT & listVT) {
	VatTu vatTu;
	for (;;) {
		clrscr(); // Xoa man hinh
		cout << "Nhap ma vat tu can hieu chinh (Nhap X de thoat): ";
		getline(cin, vatTu.maVatTu);

		// Kiểm tra chiều dài mã vật tư
		if (vatTu.maVatTu.length() > 10) {
			ThongBao("Ma vat tu co toi da 10 ki tu");
			continue;
		}

		if (vatTu.maVatTu == "X" || vatTu.maVatTu == "x")
			return;

		int viTri = TimVatTuTheoMa(listVT, vatTu.maVatTu);
		if (viTri == -1) // Tim thay
			ThongBao("Khong tim thay ma vat tu");
		else { // Khong tim thay
			cout << "Nhap ten vat tu moi: ";
			getline(cin, vatTu.tenVatTu);
			cout << "Don vi tinh moi: ";
			getline(cin, vatTu.donViTinh);
			
			// Sua thong tin trong bo nho
			listVT.list[viTri]->tenVatTu = vatTu.tenVatTu;
			listVT.list[viTri]->donViTinh = vatTu.donViTinh;

			ThongBao("Da hieu chinh vat tu");
		}
	}
}

void LietKeVatTu(ListVT listVT) { // Liệt kê vật tư theo tên tăng dần
	clrscr(); // Xoa man hinh
	ListVT listTam;
	
	// Copy danh sach con tro cac vat tu
	listTam.n = listVT.n;
	for (int i = 0; i < listTam.n; i++)
		listTam.list[i] = listVT.list[i];

	// Sap xep
	for (int i = 0; i < listTam.n; i++)
		for (int j = i + 1; j < listTam.n; j++)
			if (listTam.list[i]->tenVatTu.compare(listTam.list[j]->tenVatTu) > 0) {
				VatTu* vatTu = listTam.list[i];
				listTam.list[i] = listTam.list[j];
				listTam.list[j] = vatTu;
			}

	cout << left << setw(16) << "Ma vat tu" 
		<< left << setw(16) << "Ten vat tu" 
		<< left << setw(16) << "Don vi tinh" 
		<< left << setw(16) << "So luong ton" << endl;
	for (int i = 0; i < listTam.n; i++)
		cout << left << setw(16) << listTam.list[i]->maVatTu 
			<< left << setw(16) << listTam.list[i]->tenVatTu 
			<< left << setw(16) << listTam.list[i]->donViTinh 
			<< left << setw(16) << listTam.list[i]->soLuongTon << endl;
	_getch();
}

void ThemNhanVien(BSTNVRoot & listNV) {
	NhanVien nhanVien;
	string maNhanVien;
	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien (Nhap X de ket thuc): ";
		getline(cin, maNhanVien);

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			nhanVien.maNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			ThongBao("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Kiem tra trung ma nhan vien
		if (TimNodeBSTNV(listNV, nhanVien.maNhanVien) != NULL) {
			ThongBao("Trung ma nhan vien");
			continue;
		}

		cout << "Nhap ho: ";
		getline(cin, nhanVien.ho);
		cout << "Nhap ten: ";
		getline(cin, nhanVien.ten);

		nhanVien.listHD = NULL;

		// Them vao cay nhi phan tim kiem
		ThemNodeBSTNV(listNV, TaoNodeBSTNV(nhanVien));
	}
}

void XoaNhanVien(BSTNVRoot & listNV) {
	string maNhanVien;
	int iMaNhanVien;
	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien can xoa (Nhap X de ket thuc): ";
		getline(cin, maNhanVien);

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			iMaNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			ThongBao("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Kiem tra ton tai ma nhan vien
		if (TimNodeBSTNV(listNV, iMaNhanVien) == NULL) {
			ThongBao("Ma nhan vien khong ton tai");
			continue;
		}

		// Xoa nhan vien
		XoaNodeBSTNV(listNV, iMaNhanVien);
	}
}

void HieuChinhNhanVien(BSTNVRoot & listNV) {
	NhanVien nhanVien;
	string maNhanVien;
	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien can hieu chinh (Nhap X de ket thuc): ";
		getline(cin, maNhanVien);

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			nhanVien.maNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			ThongBao("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		BSTNVNode* node = TimNodeBSTNV(listNV, nhanVien.maNhanVien);
		if (node == NULL) {
			ThongBao("Ma nhan vien khong ton tai");
			continue;
		}

		cout << "Nhap ho moi: ";
		getline(cin, nhanVien.ho);
		cout << "Nhap ten moi: ";
		getline(cin, nhanVien.ten);

		node->data.ho = nhanVien.ho;
		node->data.ten = nhanVien.ten;
	}
}

void InNhanVien(BSTNVRoot listNV) {
	int n = 0;
	int m = 0;

	// Copy danh sach nhan vien
	NhanVien* mangNV = new NhanVien[SoPhanTuBSTNV(listNV)];
	LayMangBSTNV(listNV, mangNV, n);
	

	// Sap xep ten tang dan va ho tang dan
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (mangNV[i].ten.compare(mangNV[j].ten) < 0) {
				NhanVien nvTam = mangNV[i];
				mangNV[i] = mangNV[j];
				mangNV[j] = nvTam;
			}
			else if (mangNV[i].ten.compare(mangNV[j].ten) == 0)
				if (mangNV[i].ho.compare(mangNV[j].ho) < 0) {
					NhanVien nvTam = mangNV[i];
					mangNV[i] = mangNV[j];
					mangNV[j] = nvTam;
				}
		}

	// Xuat ra man hinh
	clrscr(); // Xoa man hinh
	cout << left << setw(20) << "Ma nhan vien" 
		<< left << setw(20) << "Ho" 
		<< left << setw(20) << "Ten" << endl;
	for (int i = 0; i < n; i++) {
		cout << left << setw(20) << mangNV[i].maNhanVien
			<< left << setw(20) << mangNV[i].ho
			<< left << setw(20) << mangNV[i].ten << endl;
	}

	_getch();
}

bool NamNhuan(int nam)
{
	return(((nam % 4 == 0) && (nam % 100 != 0)) || (nam % 400 == 0));
}

bool NgayHopLe(Ngay ngay)
{
	int ngayTrongThang[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (ngay.thang < 0 || ngay.thang > 12)
		return false;

	//Năm nhuận tháng 2 29 ngày:
	if (NamNhuan(ngay.nam))
		ngayTrongThang[1]++;

	return(ngay.ngay > 0 && ngay.ngay <= ngayTrongThang[ngay.thang - 1]);
}

// http://mathforum.org/library/drmath/view/66535.html
int TongSoNgay(Ngay ngay) { // Tinh tong so ngay so voi 0/00/0000
	if (ngay.thang < 3) {
		ngay.nam--;
		ngay.thang += 12;
	}
	return 365 * ngay.nam + ngay.nam / 4 - ngay.nam / 100 + ngay.nam / 400 + (153 * ngay.thang - 457) / 5 + ngay.ngay - 306;
}

int KhoangCachNgay(Ngay ngay1, Ngay ngay2) { // Tinh khoang cach so ngay giua 2 ngay
	return TongSoNgay(ngay2) - TongSoNgay(ngay1);
}

Ngay NgayHienTai() {
	Ngay ngay;

	// Lay thoi gian hien tai cua may
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	ngay.ngay = timeinfo->tm_mday;
	ngay.thang = timeinfo->tm_mon + 1;
	ngay.nam = timeinfo->tm_year + 1900;

	return ngay;
}

void NhapNgay(Ngay & ngay) {
	for (;;) {
		do {
			cin.clear();
			cin.ignore();
			cout << "Nhap ngay: ";
			cin >> ngay.ngay;

			// Kiểm tra
			if (cin.fail() || ngay.ngay < 1 || ngay.ngay > 31) {
				ThongBao("Vui long nhap ngay tu 1 den 31.");
				continue;
			}
		} while (cin.fail() || ngay.ngay < 1 || ngay.ngay > 31);

		do {
			cin.clear();
			cin.ignore();
			cout << "Nhap thang: ";
			cin >> ngay.thang;

			// Kiểm tra
			if (cin.fail() || ngay.thang < 1 || ngay.thang > 12) {
				ThongBao("Vui long nhap thang tu 1 den 12.");
				continue;
			}
		} while (cin.fail() || ngay.thang < 1 || ngay.thang > 12);

		do {
			cin.clear();
			cin.ignore();
			cout << "Nhap nam: ";
			cin >> ngay.nam;

			// Kiểm tra
			if (cin.fail()) {
				ThongBao("Vui long nhap so nguyen");
				continue;
			}
		} while (cin.fail());

		// Ngay thang khong hop le
		if (!NgayHopLe(ngay)) {
			ThongBao("Ngay nhap khong hop le");
			continue;
		}

		// Ngay thang trong tuong lai
		if (KhoangCachNgay(ngay, NgayHienTai()) < 0) {
			ThongBao("Ngay nhap vuot qua ngay hien tai.");
			continue;
		}

		break;
	};
}

void LapHoaDon(BSTNVRoot listNV, ListVT listVT) { // Nhap nhan vien -> hoa don
	HoaDon hoaDon;
	char loaiHoaDon;
	int iMaNhanVien;
	string maNhanVien;
	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien (Nhap X de ket thuc): ";
		getline(cin, maNhanVien);

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		if (maNhanVien == "") {
			ThongBao("Vui long nhap ma nhan vien");
			continue;
		}

		// Kiểm tra và chuyển dữ liệu nhập  (Mã nhân viên)
		try {
			iMaNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			ThongBao("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Tìm nhân viên trong danh sách
		BSTNVNode* nodeNhanVien = TimNodeBSTNV(listNV, iMaNhanVien);
		if (nodeNhanVien == NULL) {
			ThongBao("Ma nhan vien khong ton tai");
			continue;
		}

		do {
			cout << "Nhap so hoa don: ";
			getline(cin, hoaDon.soHoaDon);
		} while (hoaDon.soHoaDon == "");// kiem tra trung hd


		do {
			cout << "Nhap loai hoa don (N: nhap, X: xuat): ";
			cin >> hoaDon.loai;
			if (hoaDon.loai != 'X' && hoaDon.loai != 'N')
				ThongBao("Vui long chi nhap N hoac X cho loai hoa don.");
		} while (hoaDon.loai != 'X' && hoaDon.loai != 'N');

		cout << "Nhap ngay lap: " << endl;
		NhapNgay(hoaDon.ngayLap);

		hoaDon.listCTHD.n = 0;

		for (;;) {
			ChiTietHoaDon cthd;

			clrscr(); // Xoa man hinh
			cout << "So vat tu da nhap: " << hoaDon.listCTHD.n << endl;
			// Nhap du lieu
			cout << "Nhap ma vat tu (Nhap X de ket thuc): ";
			cin >> cthd.maVatTu;

			// Thoat neu nhap x
			if (cthd.maVatTu == "X" || cthd.maVatTu == "x")
				break;

			// Kiem tra trung ma vat tu
			int viTri = TimVatTuTheoMa(listVT, cthd.maVatTu);
			if (viTri == -1) {
				ThongBao("Vat tu khong ton tai");
				continue;
			}

			// Trạng thái
			cthd.trangThai = 1;

			// Kiểm tra dữ liệu nhập (số lượng)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap so luong: ";
				cin >> cthd.soLuong;

				// Kiểm tra nhập số nguyên
				if (cin.fail()) {
					ThongBao("Vui long chi nhap so nguyen.");
					continue;
				}

				// Kiểm tra số lượng tồn
				if (cthd.soLuong > listVT.list[viTri]->soLuongTon && cthd.trangThai == 1)
					ThongBao("So luong ton khong du.");
			} while (cin.fail() || cthd.soLuong > listVT.list[viTri]->soLuongTon);

			// Kiểm tra dữ liệu nhập (đơn giá)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap don gia: ";
				cin >> cthd.donGia;

				if (cin.fail())
					ThongBao("Vui long chi nhap so nguyen.");
			} while (cin.fail());

			// Kiểm tra dữ liệu nhập (VAT)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap VAT (%): ";
				cin >> cthd.VAT;

				if (cin.fail())
					ThongBao("Vui long chi nhap so thuc.");
			} while (cin.fail());

			int i = hoaDon.listCTHD.n;
			hoaDon.listCTHD.list[i] = cthd;

			if (hoaDon.loai == 'X')
				listVT.list[viTri]->soLuongTon -= cthd.soLuong;
			else
				listVT.list[viTri]->soLuongTon += cthd.soLuong;

			hoaDon.listCTHD.list[hoaDon.listCTHD.n] = cthd;
			hoaDon.listCTHD.n++;
		}

		ThemNodeLListHD(nodeNhanVien->data.listHD, TaoNodeLListHD(hoaDon));
	}
}

void TraHang(BSTNVRoot & listNV, ListVT listVT) {
	int maNhanVien;
	string soHoaDon;
	LListHDNode* hoaDon;

	for (;;) {
		clrscr();
		cout << "Nhap so hoa don (Nhap X de thoat): ";
		getline(cin, soHoaDon);
		if (soHoaDon == "x" || soHoaDon == "X")
			return;
		hoaDon = TimNodeHDBSTNV(listNV, soHoaDon);
		
		if (hoaDon == NULL) {
			ThongBao("Ma hoa don khong ton tai");
			continue;
		}

		if (KhoangCachNgay(hoaDon->data.ngayLap, NgayHienTai()) > 3) {
			ThongBao("Da qua han tra hang");
			continue;
		}

		break;
	};

	clrscr();
	cout << left << setw(10) << "STT"
		<< left << setw(20) << "Ma vat tu"
		<< left << setw(20) << "So luong"
		<< left << setw(20) << "So luong"
		<< left << setw(20) << "VAT"
		<< left << setw(20) << "Trang thai" << endl;
	for (int i = 0; i < hoaDon->data.listCTHD.n; i++) {
		ChiTietHoaDon cthd = hoaDon->data.listCTHD.list[i];
		cout << left << setw(10) << i + 1
			<< left << setw(20) << cthd.maVatTu
			<< left << setw(20) << cthd.soLuong
			<< left << setw(20) << cthd.donGia
			<< left << setw(20) << cthd.VAT
			<< left << setw(20) << cthd.trangThai << endl;
	}

	string stt;
	cout << "Nhap so thu tu hang hoa muon tra (Nhap X de ket thuc): ";
	for (;;) {
		cin >> stt;
		if (stt == "x" || stt == "X")
			return;
		
		int istt;
		try {
			istt = stoi(stt);
		}
		catch (invalid_argument e) {
			ThongBao("STT khong hop le");
			continue;
		}

		if (istt < 1 || istt > hoaDon->data.listCTHD.n) {
			ThongBao("STT khong hop le");
			continue;
		}

		if (hoaDon->data.listCTHD.list[istt - 1].trangThai == 0) {
			ThongBao("Hang hoa da duoc tra truoc do.");
			continue;
		}

		hoaDon->data.listCTHD.list[istt - 1].trangThai = 0;
	}
	ThongBao("Da tra hang");
}

void InHoaDon(BSTNVRoot listNV) {
	int maNhanVien;
	string soHoaDon;
	BSTNVNode* nhanVien;
	LListHDNode* hoaDon;

	clrscr();
	cout << "Nhap so hoa don: ";
	cin >> soHoaDon;

	hoaDon = TimNodeHDBSTNV(listNV, soHoaDon);
	if (hoaDon == NULL) {
		ThongBao("Khong tim thay hoa don");
		return;
	}

	cout << "So hoa don: " << hoaDon->data.soHoaDon << endl
		<< "Ngay lap: " << hoaDon->data.ngayLap.ngay << "/" << hoaDon->data.ngayLap.thang << "/" << hoaDon->data.ngayLap.nam << endl
		<< "Loai: " << hoaDon->data.loai << endl << endl;

	cout << left << setw(10) << "STT"
		<< left << setw(20) << "Ma vat tu"
		<< left << setw(20) << "So luong"
		<< left << setw(20) << "So luong"
		<< left << setw(20) << "VAT"
		<< left << setw(20) << "Trang thai" << endl;
	for (int i = 0; i < hoaDon->data.listCTHD.n; i++) {
		ChiTietHoaDon cthd = hoaDon->data.listCTHD.list[i];
		cout << left << setw(10) << i + 1
			<< left << setw(20) << cthd.maVatTu
			<< left << setw(20) << cthd.soLuong
			<< left << setw(20) << cthd.donGia
			<< left << setw(20) << cthd.VAT
			<< left << setw(20) << cthd.trangThai << endl;
	}

	_getch();
}

void LietKeHoaDon(BSTNVRoot listNV) {
	string maNhanVien;
	int iMaNhanVien;
	BSTNVNode* nhanVien;
	Ngay ngayBD;
	Ngay ngayKT;

	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien (Nhap X de ket thuc): ";
		getline(cin, maNhanVien);

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập  (Mã nhân viên)
		try {
			iMaNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			ThongBao("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Tìm nhân viên trong danh sách
		nhanVien = TimNodeBSTNV(listNV, iMaNhanVien);
		if (nhanVien == NULL) {
			ThongBao("Ma nhan vien khong ton tai");
			continue;
		}

		for (;;) {
			cout << "Nhap ngay bat dau: " << endl;
			NhapNgay(ngayBD);
			cout << "Nhap ngay ket thuc: " << endl;
			NhapNgay(ngayKT);

			if (KhoangCachNgay(ngayBD, ngayKT) < 0) { // kiem tra ngay bat dau & ngay ket thuc
				ThongBao("Ngay ket thuc phai sau ngay bat dau");
				continue;
			}
			break;
		}

		LListHD hoaDon = nhanVien->data.listHD;

		if (KhoangCachNgay(ngayBD, hoaDon->data.ngayLap) >= 0 && KhoangCachNgay(hoaDon->data.ngayLap, ngayKT) >= 0) {
			cout << left << setw(20) << "So hoa don"
				<< left << setw(20) << "Ngay lap"
				<< left << setw(20) << "Loai hoa don" << endl;
			while (hoaDon != NULL) {
				cout << left << setw(20) << hoaDon->data.soHoaDon
					<< left << setw(20) << hoaDon->data.ngayLap.ngay << "/" << hoaDon->data.ngayLap.thang << "/" << hoaDon->data.ngayLap.nam
					<< left << setw(20) << hoaDon->data.loai << endl;
				hoaDon = hoaDon->pNext;
			}
		}
		_getch();
	}
}

int DoanhThuHoaDon(HoaDon hoaDon) {
	if (hoaDon.loai == 'N')
		return 0;
	int doanhThu = 0;
	for (int i = 0; i < hoaDon.listCTHD.n; i++) {
		ChiTietHoaDon cthd = hoaDon.listCTHD.list[i];
		if (cthd.trangThai == 1)
			doanhThu += cthd.donGia * cthd.soLuong;
			// doanhThu += cthd.donGia * cthd.soLuong * (1 + cthd.VAT/100);
	}
	return doanhThu;
}

void DoanhThu(BSTNVRoot L, int doanhThu[12], int nam) {
	if (L == NULL)
		return;

	LListHDNode* nodeHD = L->data.listHD;
	while (nodeHD != NULL) {
		if (nodeHD->data.ngayLap.nam == nam)
			doanhThu[nodeHD->data.ngayLap.thang - 1] += DoanhThuHoaDon(nodeHD->data);
		nodeHD = nodeHD->pNext;
	}
	DoanhThu(L->pLeft, doanhThu, nam);
	DoanhThu(L->pRight, doanhThu, nam);
}

void ThongKeDoanhThu(BSTNVRoot listNV) {
	clrscr();
	int nam;
	int doanhThu[12]{ 0,0,0,0,0,0,0,0,0,0,0,0 };

	cout << "Nhap nam: ";
	cin >> nam;

	DoanhThu(listNV, doanhThu, nam);

	clrscr();
	cout << "THONG KE DOANH THU NAM " << nam << endl <<endl;
	cout << left << setw(20) << "Thang"
		<< left << setw(20) << "Doanh thu" << endl;
	for (int i = 0; i < 12; i++) {
		cout << left << setw(20) << i + 1
			<< left << setw(20) << doanhThu[i] << endl;
	}

	_getch();
}

int main() {
	LListHD listHD = NULL;
	ListVT listVT;
	BSTNVRoot listNV = NULL;
	listVT.n = 0;
	int itemDaChon;

	LListHD hd;

	for (;;) {
		itemDaChon = ChonMenuChinh(listThucDon);
		switch (itemDaChon)
		{
		case 1:
			NhapVatTu(listVT);
			break;
		case 2:
			XoaVatTu(listVT);
			break;
		case 3:
			HieuChinhVatTu(listVT);
			break;
		case 4:
			LietKeVatTu(listVT);
			break;
		case 5:
			ThemNhanVien(listNV);
			break;
		case 6:
			XoaNhanVien(listNV);
			break;
		case 7:
			HieuChinhNhanVien(listNV);
			break;
		case 8:
			InNhanVien(listNV);
			break;
		case 9:
			LapHoaDon(listNV, listVT);
			break;
		case 10:
			TraHang(listNV, listVT);
			break;
		case 11:
			InHoaDon(listNV);
			break;
		case 12:
			LietKeHoaDon(listNV);
			break;
		case 13:
			ThongKeDoanhThu(listNV);
			break;
		case 15:
			return 0;
			break;
		default:
			ThongBao("Chuc nang chua hoan thanh");
			break;
		}
	}

	return 0;
}