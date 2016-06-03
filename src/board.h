#pragma once

enum PieceType : uint32 {
	PieceType_Pawn,       // ����
	PieceType_Lance,      // ����
	PieceType_Knight,     // �j�n
	PieceType_Silver,     // �⏫
	PieceType_Bishop,     // �p�s
	PieceType_Rook,       // ���
	PieceType_Gold,       // ����
	PieceType_King,       // ����
	PieceType_ProPawn,    // ����
	PieceType_ProLance,   // ����
	PieceType_ProKnight,  // ���j
	PieceType_ProSilver,  // ����
	PieceType_Horse,      // ���n
	PieceType_Dragon,     // ����
	PieceType_Types,      // ��̎�ސ�
	PieceType_Pro   = PieceType_ProPawn,     // ����
	PieceType_Begin = PieceType_Pawn,        // �C�e���[�V�����p(begin)
	PieceType_End   = PieceType_Dragon + 1,  // �C�e���[�V�����p(end)
	PieceType_UnPromotedBegin = PieceType_Pawn,       // ���ł͂Ȃ���̃C�e���[�V�����p(begin)
	PieceType_UnPromotedEnd   = PieceType_King + 1,   // ���ł͂Ȃ���̃C�e���[�V�����p(end)
	PieceType_PromotedBegin   = PieceType_ProPawn,    // ����̃C�e���[�V�����p(begin)
	PieceType_PromotedEnd     = PieceType_Dragon + 1, // ����̃C�e���[�V�����p(end)
};

template <typename T> inline size_t enum_size(T e) { return (size_t)T::Size; }

class Board {

	friend ostream& operator << (ostream& os, const Board& board);

public:
	static shared_ptr<Board> current() {
		return current_;
	}

	static void current(shared_ptr<Board> value) {
		current_ = value;
	}

public:
	Board();
	Board(string sfen_board, string sfen_turn, string sfen_captured);

private:
	static shared_ptr<Board> current_;

	// BitBoard�̔z�u D:�i S:��
	//      9S    8S    7S    6S    5S    4S    3S    2S    1S
	// 1D [0:00][0:09][0:18][0:27][0:36][1:00][1:09][1:18][1:27]
	// 2D [0:01][0:10][0:19][0:28][0:37][1:01][1:10][1:19][1:28]
	// 3D [0:02][0:11][0:20][0:29][0:38][1:02][1:11][1:20][1:29]
	// 4D [0:03][0:12][0:21][0:30][0:39][1:03][1:12][1:21][1:30]
	// 5D [0:04][0:13][0:22][0:31][0:40][1:04][1:13][1:22][1:31]
	// 6D [0:05][0:14][0:23][0:32][0:41][1:05][1:14][1:23][1:32]
	// 7D [0:06][0:15][0:24][0:33][0:42][1:06][1:15][1:24][1:33]
	// 8D [0:07][0:16][0:25][0:34][0:43][1:07][1:16][1:25][1:34]
	// 9D [0:08][0:17][0:26][0:35][0:44][1:08][1:17][1:26][1:35]

	// �Տ�̋����\��BitBoard
	// [���][��ԍ�(PeaceType)]
	uint64 piece_pos_[4][PieceType_Types][2];

	// ������
	int piece_captured[2][PieceType_Gold + 1];

};

ostream& operator << (ostream& os, const Board& board);
inline ostream& operator << (ostream& os, const shared_ptr<Board> board) { return operator << (os, *board); }
