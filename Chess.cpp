#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

class ChessBoard {
private:
    static const int BOARD_SIZE = 8;
    static const char EMPTY_SQUARE = '.';

    static const char KING = 'K';;
    static const char ROOK = 'R';
    static const char BISHOP = 'B';
    static const char KNIGHT = 'N';
    static const char PAWN = 'P';

    static const char BLACK_KING = 'k';
    static const char BLACK_ROOK = 'r';
    static const char BLACK_BISHOP = 'b';
    static const char BLACK_KNIGHT = 'n';
    static const char BLACK = 'p';

    static const int CHECKMATE_POINTS = 20;
    static const int CHECK_POINTS = 15;
    static const int CAPTURE_ROOK_POINTS = 5;
    static const int CAPTURE_BISHOP_POINTS = 4;
    static const int CAPTURE_KNIGHT_POINTS = 3;
    static const int CAPTURE_PAWN_POINTS = 1;

    unsigned long seed = 1;

    vector<vector<char>> board;

    struct Move {
        char piece;
        int fromRow, fromCol, toRow, toCol;
        int movePoints = 0; 
        char check = '.';
    };

    int my_rand(int del) {
        seed = seed * 1103515245 + 12345;
        return (seed / 65536) % (2 * del);     
    }

    int BlackKingRow = my_rand(1);
    int BlackKingCol = my_rand(4);
    int WhiteKingRow = my_rand(1) + 6;
    int WhiteKingCol = my_rand(4);

    void initializeBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            vector<char> row(BOARD_SIZE, EMPTY_SQUARE);
            board.push_back(row);
        }
    }

    bool isValidPosition(int row, int col) {
        return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
    }

    // ������� ��� ��������� ��������� ����� ��� �����
    vector<Move> generatePawnMoves(int row, int col, bool isWhite) {
        vector<Move> possibleMoves;
        int direction = isWhite ? -1 : 1; // ����������� �������� �����

        // �������� ����������� �������� �� ���� ������ �����
        int newRow = row + direction;
        int newCol = col;
        if (newRow >= 0 && newRow < BOARD_SIZE && board[newRow][newCol] == '.') {
            Move move;
            move.piece = '\0';
            move.fromRow = row;
            move.fromCol = col;
            move.toRow = newRow;
            move.toCol = newCol;
            move.movePoints = 0;
            possibleMoves.push_back(move);
        }

        // �������� ����������� �������� �� ��� ������ ����� (��������� ���)
        if ((isWhite && row == 6)) {
            newRow = row + 2 * direction;
            newCol = col;
            if (newRow >= 0 && newRow < BOARD_SIZE && board[newRow][newCol] == '.' &&
                board[row + direction][col] == '.') {
                Move move;
                move.piece = '\0';
                move.fromRow = row;
                move.fromCol = col;
                move.toRow = newRow;
                move.toCol = newCol;
                move.movePoints = 0;
                possibleMoves.push_back(move);
            }
        }
        char piece;
        // �������� ����������� ������� ������ �����
        newRow = row + direction;
        newCol = col - 1;
        
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE){
            if (board[newRow][newCol] != '.' && ((isWhite && islower(board[newRow][newCol])))) {
                piece = board[newRow][newCol];
                Move move;
                move.piece = '\0';
                move.fromRow = row;
                move.fromCol = col;
                move.toRow = newRow;
                move.toCol = newCol;
                switch (piece) {
                case 'k': move.movePoints = CHECK_POINTS; break;
                case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                default: move.movePoints = 0; break;
                }
                if (canPieceAttackKing(PAWN, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING))
                    move.check = '+';
                    move.movePoints += CHECK_POINTS;
                possibleMoves.push_back(move);
            }
        }

        // �������� ����������� ������� ������ ������
        newRow = row + direction;
        newCol = col + 1;

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (board[newRow][newCol] != '.' && ((isWhite && islower(board[newRow][newCol])))) {
                piece = board[newRow][newCol];
                Move move;
                move.piece = 'P';
                move.fromRow = row;
                move.fromCol = col;
                move.toRow = newRow;
                move.toCol = newCol;
                switch (piece) {
                case 'k': move.movePoints = CHECK_POINTS; break;
                case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                default: move.movePoints = 0; break;
                }
                if (canPieceAttackKing(PAWN, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING))
                    move.movePoints += CHECK_POINTS;
                    move.check = '+';
                possibleMoves.push_back(move);
            }
        }

        return possibleMoves;
    }// ������ 

    // ������� ��� ��������� ��������� ����� ��� ����
    vector<Move> generateKnightMoves(int row, int col, bool isWhite) {
        vector<Move> possibleMoves;

        int directions[8][2] = {
            {-2, -1}, {-2, 1},
            {-1, -2}, {-1, 2},
            {1, -2}, {1, 2},
            {2, -1}, {2, 1}
        };

        for (int i = 0; i < 8; ++i) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];

            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                char piece = board[newRow][newCol];
                if (piece == '.') {
                    Move move;
                    move.piece = 'N';
                    move.fromRow = row;
                    move.fromCol = col;
                    move.toRow = newRow;
                    move.toCol = newCol;
                    move.movePoints = 0; // 2 ����� �� ��� �����
                    if (canPieceAttackKing(KNIGHT, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                        move.check = '+';
                        move.movePoints += CHECK_POINTS;
                    }
                    possibleMoves.push_back(move);
                }
                else {
                    if (isWhite && islower(piece)) {
                        Move move;
                        move.piece = 'N';
                        move.fromRow = row;
                        move.fromCol = col;
                        move.toRow = newRow;
                        move.toCol = newCol;
                        switch (piece) {
                        case 'k': move.movePoints = CHECK_POINTS; break;
                        case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                        case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                        case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                        case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                        default: move.movePoints = 0; break;
                        }
                        if (canPieceAttackKing(KNIGHT, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                            move.check = '+';
                            move.movePoints += CHECK_POINTS;
                        }
                        possibleMoves.push_back(move);
                    }
                }
            }
        }

        return possibleMoves;
    }

    // ������� ��� ��������� ��������� ����� ��� �����
    vector<Move> generateBishopMoves(int row, int col, bool isWhite) {
        vector<Move> possibleMoves;

        int directions[4][2] = {
            {-1, -1}, {-1, 1},
            {1, -1}, {1, 1}
        };

        for (int i = 0; i < 4; ++i) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];

            while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                char piece = board[newRow][newCol];
                if (piece == '.') {
                    Move move;
                    move.piece = 'B';
                    move.fromRow = row;
                    move.fromCol = col;
                    move.toRow = newRow;
                    move.toCol = newCol;
                    move.movePoints = 0;
                    if (canPieceAttackKing(BISHOP, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                        move.movePoints += CHECK_POINTS;
                        move.check = '+';
                    }
                    possibleMoves.push_back(move);
                    newRow += directions[i][0];
                    newCol += directions[i][1];
                }
                else {
                    if (isWhite && islower(piece)) {
                        Move move;
                        move.piece = 'B';
                        move.fromRow = row;
                        move.fromCol = col;
                        move.toRow = newRow;
                        move.toCol = newCol;
                        switch (piece) {
                        case 'k': move.movePoints = CHECK_POINTS; break;
                        case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                        case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                        case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                        case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                        default: move.movePoints = 0; break;
                        }
                        if (canPieceAttackKing(BISHOP, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                            move.movePoints += CHECK_POINTS;
                            move.check = '+';
                        }
                        possibleMoves.push_back(move);
                    }
                    break;
                }
            }
        }

        return possibleMoves;
    }

    // ������� ��� ��������� ��������� ����� ��� �����
    vector<Move> generateRookMoves(int row, int col, bool isWhite) {
        vector<Move> possibleMoves;

        int directions[4][2] = {
            {-1, 0}, {1, 0}, // �������� �� ���������
            {0, -1}, {0, 1}  // �������� �� �����������
        };

        for (int i = 0; i < 4; ++i) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];

            while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                char piece = board[newRow][newCol];
                if (piece == '.') {
                    Move move;
                    move.piece = 'R';
                    move.fromRow = row;
                    move.fromCol = col;
                    move.toRow = newRow;
                    move.toCol = newCol;
                    move.movePoints = 0;
                    if (canPieceAttackKing(ROOK, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                        move.movePoints += CHECK_POINTS;
                        move.check = '+';
                    }
                    possibleMoves.push_back(move);
                    newRow += directions[i][0];
                    newCol += directions[i][1];
                }
                else {
                    if ((isWhite && islower(piece))) {
                        Move move;
                        move.piece = 'R';
                        move.fromRow = row;
                        move.fromCol = col;
                        move.toRow = newRow;
                        move.toCol = newCol;
                        switch (piece) {
                        case 'k': move.movePoints = CHECK_POINTS; break;
                        case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                        case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                        case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                        case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                        default: move.movePoints = 0;
                        }
                        if (canPieceAttackKing(ROOK, move.toRow, move.toCol, BlackKingRow, BlackKingCol, BLACK_KING)) {
                            move.movePoints += CHECK_POINTS;
                            move.check = '+';
                        }
                        possibleMoves.push_back(move);
                    }
                    break;
                }
            }
        }

        return possibleMoves;
    }

    // ������� ��� ��������� ��������� ����� ��� ������ (������)
    vector<Move> generateKingMoves(int row, int col, bool isWhite) {
        vector<Move> possibleMoves;

        int directions[8][2] = {
            {-1, 0}, {1, 0}, // ������������ ����
            {0, -1}, {0, 1}, // �������������� ����
            {-1, -1}, {-1, 1}, // ������������ ����
            {1, -1}, {1, 1}
        };

        for (int i = 0; i < 8; ++i) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];

            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                char piece = board[newRow][newCol];
                if (piece == '.' || (isWhite && islower(piece)) || (!isWhite && isupper(piece))) {
                    Move move;
                    move.piece = 'K';
                    move.fromRow = row;
                    move.fromCol = col;
                    move.toRow = newRow;
                    move.toCol = newCol;
                    move.movePoints = 0;
                    possibleMoves.push_back(move);
                }
                else {
                    if ((isWhite && islower(piece)) && (!isKingUnderAttack(newRow, newCol, KING))) {
                        Move move;
                        move.piece = 'K';
                        move.fromRow = row;
                        move.fromCol = col;
                        move.toRow = newRow;
                        move.toCol = newCol;
                        switch (piece) {
                        case 'r': move.movePoints = CAPTURE_ROOK_POINTS; break;
                        case 'b': move.movePoints = CAPTURE_BISHOP_POINTS; break;
                        case 'n': move.movePoints = CAPTURE_KNIGHT_POINTS; break;
                        case 'p': move.movePoints = CAPTURE_PAWN_POINTS; break;
                        default: move.movePoints = 0; break;
                        }
                        possibleMoves.push_back(move);
                    }
                }
            }
        }

        return possibleMoves;
    } // ������� ������� // ������

    // ������� ��� ��������� ���� ��������� ����� �� �����
    vector<Move> generatePossibleMoves() {
        vector<Move> allMoves;

        // ������� ������ �� �����
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                char piece = board[row][col];
                if (piece == '.') // ���������� ������ ������
                    continue;

                // �������� ����� ������ (����� ��� ������)
                bool isWhite = (piece >= 'A' && piece <= 'Z');

                if (!isWhite)
                    continue;

                if (piece == 'P') {
                    vector<Move> pawnMoves = generatePawnMoves(row, col, isWhite);
                    if (pawnMoves.size() > 0) {
                        allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
                    }

                }
                else if (piece == 'N') {
                    vector<Move> knightMoves = generateKnightMoves(row, col, isWhite);
                    if (knightMoves.size() > 0)
                        allMoves.insert(allMoves.end(), knightMoves.begin(), knightMoves.end());
                }
                else if (piece == 'B') {
                    vector<Move> bishopMoves = generateBishopMoves(row, col, isWhite);
                    if (bishopMoves.size() > 0)
                        allMoves.insert(allMoves.end(), bishopMoves.begin(), bishopMoves.end());
                }
                else if (piece == 'R') {
                    vector<Move> rookMoves = generateRookMoves(row, col, isWhite);
                    if (rookMoves.size() > 0)
                        allMoves.insert(allMoves.end(), rookMoves.begin(), rookMoves.end());
                }
                else if (piece == 'K') {
                    vector<Move> kingMoves = generateKingMoves(row, col, isWhite);
                    if (kingMoves.size() > 0)
                        allMoves.insert(allMoves.end(), kingMoves.begin(), kingMoves.end());
                }
            }
        }

        return allMoves;
    }

public:
    ChessBoard() {
        initializeBoard();
    }

    void printBoard() {
        // ����� ��������� ������� (a-h)
        cout << "   ";
        for (char c = 'a'; c <= 'h'; ++c) {
            cout << c << ' ';
        }
        cout << endl;

        // ����� ������� ������� �����
        cout << "  ";
        for (int i = 0; i < BOARD_SIZE * 2 + 1; ++i) {
            cout << '-';
        }
        cout << endl;

        // ����� ����� � �������� � ��������
        for (int i = 0; i < BOARD_SIZE; ++i) {
            // ����� ������ ������
            cout << BOARD_SIZE - i << "| ";

            // ����� ��������� ����� � �����
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cout << board[i][j] << ' ';
            }

            // ���������� ����� � �������
            cout << '|' << BOARD_SIZE - i << endl;
        }

        // ����� ������ ������� �����
        cout << "  ";
        for (int i = 0; i < BOARD_SIZE * 2 + 1; ++i) {
            cout << '-';
        }
        cout << endl;

        // ����� ��������� ������� (a-h)
        cout << "   ";
        for (char c = 'a'; c <= 'h'; ++c) {
            cout << c << ' ';
        }
        cout << endl;
    }

    bool isPositionValid(int row, int col, char figure) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
            return false;

        if (board[row][col] != EMPTY_SQUARE)
            return false;

        if (figure == PAWN && row == BOARD_SIZE - 1 || row == 0)
            return false;

        if (figure == BLACK && row == 0 || row == BOARD_SIZE - 1)
            return false;

        return true;
    }

    bool isKingUnderAttack(int kingRow, int kingCol, char king) {
        char color;
        if (king == KING)
            color = 'K';
        else color = 'k';
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                char piece = board[i][j];
                if (piece != EMPTY_SQUARE && ((king == KING && islower(piece)) || (king == BLACK_KING && isupper(piece)))) {
                    if (canPieceAttackKing(piece, i, j, kingRow, kingCol, color))
                        return true;
                }
            }
        }
        return false;
    }

    bool canPieceAttackKing(char piece, int pieceRow, int pieceCol, int kingRow, int kingCol, char kingColor) {
        if (!isValidPosition(pieceRow, pieceCol) || !isValidPosition(kingRow, kingCol)) {
            // Invalid positions, cannot attack
            return false;
        }

        switch (piece) {
        case ROOK:  // Rook
            if (pieceRow == kingRow) {
                int startCol = std::min(pieceCol, kingCol) + 1;
                int endCol = std::max(pieceCol, kingCol);
                for (int col = startCol; col < endCol; ++col) {
                    if (isValidPosition(pieceRow, col) && board[pieceRow][col] != EMPTY_SQUARE)
                        return false;
                }
                return true;
            }
            else if (pieceCol == kingCol) {
                int startRow = std::min(pieceRow, kingRow) + 1;
                int endRow = std::max(pieceRow, kingRow);
                for (int row = startRow; row < endRow; ++row)
                    if (isValidPosition(row, pieceCol) && board[row][pieceCol] != EMPTY_SQUARE)
                        return false;
                return true;
            }
            return false;

        case BISHOP:
        {
            int rowIncrement = (pieceRow < kingRow) ? 1 : -1;
            int colIncrement = (pieceCol < kingCol) ? 1 : -1;
            int row = pieceRow + rowIncrement;
            int col = pieceCol + colIncrement;

            while (row != kingRow && col != kingCol) {
                if (!isValidPosition(row, col) || board[row][col] != EMPTY_SQUARE)
                    return false;

                row += rowIncrement;
                col += colIncrement;
            }
            return (abs(pieceRow - kingRow) == abs(pieceCol - kingCol));
        }

        case KNIGHT:
            return (abs(pieceRow - kingRow) == 2 && abs(pieceCol - kingCol) == 1) ||
                (abs(pieceRow - kingRow) == 1 && abs(pieceCol - kingCol) == 2);

        case PAWN:
            return (kingRow == pieceRow - 1) && (abs(kingCol - pieceCol) == 1) && (kingColor == 'k');

        case BLACK:
            return (kingRow == pieceRow + 1) && (abs(kingCol - pieceCol) == 1) && (kingColor == 'K');

        default:
            return false;
        }
    }

    void generateRandomPosition(int numFigures) {
        srand(time(0));
        board[WhiteKingRow][WhiteKingCol] = KING;
        board[BlackKingRow][BlackKingCol] = BLACK_KING;

        for (int i = 0; i < numFigures - 2; ++i) {
            char figure;
            int row, col;
            int color = rand() % 2; // 0 - �����, 1 - ������

            switch (color) {
            case 0:
            {
                switch (rand() % 4) {
                case 0: figure = PAWN; break;
                case 1: figure = ROOK; break;
                case 2: figure = BISHOP; break;
                case 3: figure = KNIGHT; break;
                }
            } break;
            case 1:
            {
                switch (rand() % 4) {
                case 0: figure = BLACK; break;
                case 1: figure = BLACK_ROOK; break;
                case 2: figure = BLACK_BISHOP; break;
                case 3: figure = BLACK_KNIGHT; break;
                }
            }break;
            }

            if (color == 0) {
                do {
                    row = rand() % BOARD_SIZE;
                    col = rand() % BOARD_SIZE;
                } while (!isPositionValid(row, col, EMPTY_SQUARE)
                    || canPieceAttackKing(figure, row, col, BlackKingRow, BlackKingCol, 'k'));
            }
            else {
                do {
                    row = rand() % BOARD_SIZE;
                    col = rand() % BOARD_SIZE;
                } while (!isPositionValid(row, col, EMPTY_SQUARE)
                    || canPieceAttackKing(figure, row, col, WhiteKingRow, WhiteKingCol, 'K'));
            }
            board[row][col] = figure;
        }
    }

    void applyMove(int fromRow, int fromCol, int toRow, int toCol) {
        // ��������� ����� � ������������ � �����
        // �����������, move �������� ���������� ��������
        // ��������� ���, ��������:
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = '.';
    }

    vector<Move> findBestMoves() {
        vector<Move> bestMoves(3);  // ������ ��� �������� ���-3 ������ �����
        for (int i = 0; i < 3; ++i)
            bestMoves[i].movePoints = 0;

        vector<Move> possibleMoves = generatePossibleMoves();

        for (const auto& move : possibleMoves) {
            for (auto& bestMove : bestMoves) {
                if (move.movePoints >= bestMove.movePoints) {
                    bestMove = move;
                    break;
                }
            }
        }

        return bestMoves;
    }

    void findAndMakesMoves(int numOfMoves) {
        Move bestMove;
        vector<vector<char>> board_start = board;
        for (int var = 0; var < 3; ++var) {
            board = board_start;
            bestMove = findBestMoves()[var];

            if (bestMove.fromRow == -1 && bestMove.fromCol == -1 && bestMove.toRow == -1 && bestMove.toCol == -1) {
                vector<Move> nextBestMoves = findBestMoves();
                if (!nextBestMoves.empty())
                    bestMove = nextBestMoves[0];  // Choose the next best move if the current one is invalid
            }

            for (int i = 0; i < numOfMoves; ++i) {

                applyMove(bestMove.fromRow, bestMove.fromCol, bestMove.toRow, bestMove.toCol);
                if (bestMove.check == '+') {
                    cout << i + 1 << ". ";
                    cout << bestMove.piece << convertToChessNotation(bestMove.fromRow, bestMove.fromCol)
                        << "-" << convertToChessNotation(bestMove.toRow, bestMove.toCol) << bestMove.check << " ";
                    break;
                }
                else {
                    cout << i + 1 << ". ";
                    cout << bestMove.piece << convertToChessNotation(bestMove.fromRow, bestMove.fromCol)
                        << "-" << convertToChessNotation(bestMove.toRow, bestMove.toCol) << " ";
                }
                bestMove = findBestMoves()[0];

            }
            cout << "\n";
        }
    }
    string convertToChessNotation(int row, int col) {
        char file = col + 'a';
        char rank = '8' - row;

        string chessNotation;
        chessNotation += file;
        chessNotation += rank;

        return chessNotation;
    }
};



int main() {
    int NumberOfFigures;
    cout << "Write number of figures: ";
    cin >> NumberOfFigures;

    for (int i = 0; i < 1; i++) {
        ChessBoard chessBoard;
        chessBoard.generateRandomPosition(NumberOfFigures);
        chessBoard.printBoard();

        chessBoard.findAndMakesMoves(3);
    }

    return 0;
}

