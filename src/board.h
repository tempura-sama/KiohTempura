#pragma once

enum PieceType : uint32 {
	PieceType_Pawn,       // 歩兵
	PieceType_Lance,      // 香車
	PieceType_Knight,     // 桂馬
	PieceType_Silver,     // 銀将
	PieceType_Bishop,     // 角行
	PieceType_Rook,       // 飛車
	PieceType_Gold,       // 金将
	PieceType_King,       // 王将
	PieceType_ProPawn,    // 成歩
	PieceType_ProLance,   // 成香
	PieceType_ProKnight,  // 成桂
	PieceType_ProSilver,  // 成銀
	PieceType_Horse,      // 龍馬
	PieceType_Dragon,     // 龍王
	PieceType_Types,      // 駒の種類数
	PieceType_Pro   = PieceType_ProPawn,     // 成駒
	PieceType_Begin = PieceType_Pawn,        // イテレーション用(begin)
	PieceType_End   = PieceType_Dragon + 1,  // イテレーション用(end)
	PieceType_UnPromotedBegin = PieceType_Pawn,       // 成ではない駒のイテレーション用(begin)
	PieceType_UnPromotedEnd   = PieceType_King + 1,   // 成ではない駒のイテレーション用(end)
	PieceType_PromotedBegin   = PieceType_ProPawn,    // 成駒のイテレーション用(begin)
	PieceType_PromotedEnd     = PieceType_Dragon + 1, // 成駒のイテレーション用(end)
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

	// BitBoardの配置 D:段 S:筋
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

	// 盤上の駒情報を表すBitBoard
	// [手番][駒番号(PeaceType)]
	uint64 piece_pos_[4][PieceType_Types][2];

	// 持駒情報
	int piece_captured[2][PieceType_Gold + 1];

};

ostream& operator << (ostream& os, const Board& board);
inline ostream& operator << (ostream& os, const shared_ptr<Board> board) { return operator << (os, *board); }
