// Các tính năng còn thiếu
// - kiểm tra điều kiện dữ liệu nhập vào
// - các tính năng
// - kiểm tra ngày tháng năm

// - kiểm tra nhập dữ liệu


#include <iostream>
#include <string>
#include <iomanip>

#include "mylib.h"

using namespace std;

// Hằng số
const int SO_ITEM_MENU = 15;

const int MAX_VAT_TU = 100;
const int MAX_CTHD = 20;

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;

char listThucDon[SO_ITEM_MENU][50] = {  "--1.  Them vat tu",
										"--2.  Xoa vat tu",
										"--3.  Hieu Chinh Vat Tu",
										"x-4.  Liet ke danh sach vat tu", // chưa liệt kê theo thứ tự yêu cầu
										"--5.  Them nhan vien",
										"--6.  Xoa nhan vien",
										"--7.  Hieu chinh nhan vien",
										"x-8.  In danh sach nhan vien", // chưa liệt kê theo thứ tự yêu cầu
										"x-9.  Lap hoa don",
										"10. Tra hang",
										"11. In hoa don",
										"12. Liet ke hoa don theo nhan vien",
										"13. Load du lieu tu file",
										"14. Luu du lieu vao file",
										"--15. Thoat" };

// Biến
int dong = 5;
int cot = 10;

// Danh sách thực đơn

void BaoLoi(const char* s) { // Hàm thông báo lỗi
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
			BaoLoi("Key da ton tai");
	}
};

BSTNVRoot TimNodeBSTNV(BSTNVRoot & L, int key) {
	if (L == NULL)
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
		BaoLoi("Cay rong");
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
		BaoLoi("Key khong ton tai");
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

LListHDNode* TimNodeHDBSTNV(BSTNVRoot L, string key) {
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

//BSTNVRoot NodeNhoNhatBSTNV(BSTNVRoot& listNV) {
//	BSTNVRoot tmp = listNV;
//	while (tmp != NULL && tmp->pLeft != NULL)
//		tmp = tmp->pLeft;
//	return tmp;
//}

//BSTNVRoot XoaNodeBSTNV(BSTNVRoot& listNV, int key) {
//	if (listNV == NULL)
//		return listNV;
//	if (key < listNV->key) // key nhỏ hơn key của node đang xét
//		XoaNodeBSTNV(listNV->pLeft, key);
//	else if (key > listNV->key) // key nhỏ hơn key của node đang xét
//		XoaNodeBSTNV(listNV->pRight, key);
//	else { // key = listNV->key: node hiện tại là node cần xoá
//		// node có 1 con hoặc ko có node con
//		if (listNV->pLeft == NULL) { // node trái null
//			BSTNVRoot tmp = listNV->pRight;
//			delete listNV;
//			return tmp;
//		} 
//		if (listNV->pRight == NULL) { //node phải null
//			BSTNVRoot tmp = listNV->pLeft;
//			delete listNV;
//			return tmp;
//		}
//		// node có đủ 2 node con
//		BSTNVRoot tmp = NodeNhoNhatBSTNV(listNV->pRight);
//		listNV->key = tmp->key;
//		listNV->data = tmp->data;
//		listNV->pRight = XoaNodeBSTNV(listNV->pRight, listNV->key);
//	}
//	return listNV;
//}

// ++ End nhân viên ++ //

// ----------------- Các hàm chức năng ----------------- //

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
			if (itemDaChon + 1 > 1) {
				itemDaChon--;
				VeMenuChinh(listThucDon, itemDaChon);
			}
			break;
		case KEY_DOWN:
			if (itemDaChon + 1 < SO_ITEM_MENU) {
				itemDaChon++;
				VeMenuChinh(listThucDon, itemDaChon);
			}
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
		// TH danh sach day
		if (listVT.n > MAX_VAT_TU)
		{
			BaoLoi("Danh sach day");
			return;
		}

		clrscr(); // Xoa man hinh
		cout << "So vat tu da nhap: " << listVT.n << endl;
		// Nhap du lieu
		cout << "Nhap ma vat tu (Nhap X de ket thuc): ";
		cin >> vatTu.maVatTu;

		// Thoat neu nhap x
		if (vatTu.maVatTu == "X" || vatTu.maVatTu == "x")
			return;

		// Kiem tra trung ma vat tu
		if (TimVatTuTheoMa(listVT, vatTu.maVatTu) != -1) {
			BaoLoi("Trung ma vat tu");
			continue;
		}

		cout << "Nhap ten vat tu: ";
		cin >> vatTu.tenVatTu;
		cout << "Don vi tinh: ";
		cin >> vatTu.donViTinh;
		cout << "So luong ton: ";

		// Xu ly nhap so luong ton khac float
		cin >> vatTu.soLuongTon;
		if (cin.fail()) {
			BaoLoi("Vui long chi nhap so thuc.");
		}
		while (cin.fail()) {
			cin.clear();
			cin.ignore();
			cin >> vatTu.soLuongTon;
		}

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
		cin >> maVatTu;
		if (maVatTu == "X" || maVatTu == "x")
			return;
		int viTri = TimVatTuTheoMa(listVT, maVatTu);
		if (viTri == -1)
			BaoLoi("Khong tim thay ma vat tu");
		else {
			delete listVT.list[viTri];
			for (int i = viTri; i < listVT.n - 1; i++)
				listVT.list[i] = listVT.list[i + 1];
			listVT.n--;
			BaoLoi("Da xoa vat tu");
		}
	}
}

void HieuChinhVatTu(ListVT & listVT) {
	VatTu vatTu;
	for (;;) {
		clrscr(); // Xoa man hinh
		cout << "Nhap ma vat tu can hieu chinh (Nhap X de thoat): ";
		cin >> vatTu.maVatTu;
		if (vatTu.maVatTu == "X" || vatTu.maVatTu == "x")
			return;
		int viTri = TimVatTuTheoMa(listVT, vatTu.maVatTu);
		if (viTri == -1)
			BaoLoi("Khong tim thay ma vat tu");
		else {
			cout << "Nhap ten vat tu moi: ";
			cin >> vatTu.tenVatTu;
			cout << "Don vi tinh moi: ";
			cin >> vatTu.donViTinh;
			cout << "So luong ton moi: ";

			// Sua thong tin trong bo nho
			listVT.list[viTri]->tenVatTu = vatTu.tenVatTu;
			listVT.list[viTri]->donViTinh = vatTu.donViTinh;

			BaoLoi("Da hieu chinh vat tu");
		}
	}
}

void LietKeVatTu(ListVT listVT) { // Liệt kê vật tư theo tên tăng dần
	clrscr(); // Xoa man hinh
	cout << "Ma vat tu" << setw(14) << "Ten vat tu" << setw(14) << "Don vi tinh" << setw(16) << "So luong ton" << endl;
	for (int i = 0; i < listVT.n; i++)
		cout << listVT.list[i]->maVatTu << setw(13) << listVT.list[i]->tenVatTu << setw(13) << listVT.list[i]->donViTinh << setw(15) << listVT.list[i]->soLuongTon << endl;
	_getch();
}

void ThemNhanVien(BSTNVRoot & listNV) {
	NhanVien nhanVien;
	string maNhanVien;
	for (;;) {
		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien (Nhap X de ket thuc): ";
		cin >> maNhanVien;

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			nhanVien.maNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			BaoLoi("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Kiem tra trung ma nhan vien
		if (TimNodeBSTNV(listNV, nhanVien.maNhanVien) != NULL) {
			BaoLoi("Trung ma nhan vien");
			continue;
		}

		cout << "Nhap ho: ";
		cin >> nhanVien.ho;
		cout << "Nhap ten: ";
		cin >> nhanVien.ten;

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
		cin >> maNhanVien;

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			iMaNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			BaoLoi("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Kiem tra ton tai ma nhan vien
		if (false) {
			BaoLoi("Ma nhan vien khong ton tai");
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
		cin >> maNhanVien;

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập
		try {
			nhanVien.maNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			BaoLoi("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		BSTNVNode* node = TimNodeBSTNV(listNV, nhanVien.maNhanVien);
		if (node == NULL) {
			BaoLoi("Ma nhan vien khong ton tai");
			continue;
		}

		cout << "Nhap ho moi: ";
		cin >> nhanVien.ho;
		cout << "Nhap ten moi: ";
		cin >> nhanVien.ten;

		node->data.ho = nhanVien.ho;
		node->data.ten = nhanVien.ten;
	}
}

void DuyetNhanVien(BSTNVRoot listNV) {
	if (listNV != NULL) {
		DuyetNhanVien(listNV->pLeft);
		cout << listNV->data.maNhanVien << setw(13) << listNV->data.ho << setw(13) << listNV->data.ten << endl;
		DuyetNhanVien(listNV->pRight);
	}
}

void InNhanVien(BSTNVRoot listNV) {
	clrscr(); // Xoa man hinh
	cout << "Ma nhan vien" << setw(14) << "Ho" << setw(14) << "Ten" << endl;
	DuyetNhanVien(listNV);
	_getch();
}

void NhapNgay(Ngay & ngay) {
	cout << "Nhap ngay: ";
	cin >> ngay.ngay;
	cout << "Nhap thang: ";
	cin >> ngay.thang;
	cout << "Nhap nam: ";
	cin >> ngay.nam;
}

void LapHoaDon(LListHD listHD, BSTNVRoot listNV, ListVT listVT) {
	HoaDon hoaDon;
	char loaiHoaDon;
	int iMaNhanVien;
	string maNhanVien;
	for (;;) {
		// TH danh sach day
		if (hoaDon.listCTHD.n >= MAX_CTHD)
		{
			BaoLoi("Danh sach day");
			break;
		}

		clrscr(); // Xoa man hinh
		// Nhap du lieu
		cout << "Nhap ma nhan vien (Nhap X de ket thuc): ";
		cin >> maNhanVien;

		// Thoat neu nhap x
		if (maNhanVien == "X" || maNhanVien == "x")
			return;

		// Kiểm tra và chuyển dữ liệu nhập  (Mã nhân viên)
		try {
			iMaNhanVien = stoi(maNhanVien);
		}
		catch (invalid_argument e) {
			BaoLoi("Vui long chi nhap so cho ma nhan vien");
			continue;
		}

		// Tìm nhân viên trong danh sách
		BSTNVNode* nodeNhanVien = TimNodeBSTNV(listNV, iMaNhanVien);
		if (nodeNhanVien == NULL) {
			BaoLoi("Ma nhan vien khong ton tai");
			continue;
		}

		cout << "Nhap so hoa don: ";
		cin >> hoaDon.soHoaDon;

		do {
			cout << "Nhap loai hoa don (N: nhap, X: xuat): ";
			cin >> hoaDon.loai;
			if (hoaDon.loai != 'X' && hoaDon.loai != 'N')
				BaoLoi("Vui long chi nhap N hoac X cho loai hoa don.");
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
				BaoLoi("Vat tu khong ton tai");
				continue;
			}

			// Trạng thái
			if (hoaDon.loai == 'N')
				cthd.trangThai = 0;
			else if (hoaDon.loai == 'X')
				cthd.trangThai = 1;

			// Kiểm tra dữ liệu nhập (số lượng)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap so luong: ";
				cin >> cthd.soLuong;

				// Kiểm tra nhập số nguyên
				if (cin.fail()) {
					BaoLoi("Vui long chi nhap so nguyen.");
					continue;
				}

				// Kiểm tra số lượng tồn
				if (cthd.soLuong > listVT.list[viTri]->soLuongTon && cthd.trangThai == 1)
					BaoLoi("So luong ton khong du.");
			} while (cin.fail() || cthd.soLuong > listVT.list[viTri]->soLuongTon);

			// Kiểm tra dữ liệu nhập (đơn giá)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap don gia: ";
				cin >> cthd.donGia;

				if (cin.fail())
					BaoLoi("Vui long chi nhap so nguyen.");
			} while (cin.fail());

			// Kiểm tra dữ liệu nhập (VAT)
			do {
				cin.clear();
				cin.ignore();
				cout << "Nhap VAT (%): ";
				cin >> cthd.VAT;

				if (cin.fail())
					BaoLoi("Vui long chi nhap so thuc.");
			} while (cin.fail());

			int i = hoaDon.listCTHD.n;
			hoaDon.listCTHD.list[i] = cthd;

			if (cthd.trangThai == 1)
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
	BSTNVNode* nhanVien;
	LListHDNode* hoaDon;

	clrscr();
	cout << "Nhap so hoa don: ";
	cin >> soHoaDon;

	nhanVien = TimNodeBSTNV(listNV, maNhanVien);
	if (nhanVien == NULL)
		BaoLoi("Khong tim thay nhan vien");

	hoaDon = nhanVien->data.listHD;
	while (hoaDon != NULL) {
		cout << hoaDon->data.soHoaDon << endl;
		hoaDon = hoaDon->pNext;
	}

	_getch();
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
	if (hoaDon == NULL)
		BaoLoi("Khong tim thay hoa don");
	cout << hoaDon->data.soHoaDon << " - " << hoaDon->data.ngayLap.ngay <<  "/" << hoaDon->data.ngayLap.thang << "/" << hoaDon->data.ngayLap.nam;

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
			//Test0(hd);
			//Test(hd);
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
			LapHoaDon(listHD, listNV, listVT);
			break;
		case 10:
			TraHang(listNV, listVT);
			break;
		case 11:
			InHoaDon(listNV);
			break;
		case 15:
			return 0;
			break;
		default:
			BaoLoi("Chuc nang chua hoan thanh");
			break;
		}
	}

	return 0;
}