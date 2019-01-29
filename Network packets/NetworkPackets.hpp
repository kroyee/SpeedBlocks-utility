#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "NetworkPacketHandler.hpp"

// Client outgoing

PACKET(NP_Replay){};

PACKET(NP_LoginRequest) {
    uint16_t clientVersion = 0;
    uint8_t guest = 0;
    std::string nameOrHash;
};

PACKET(NP_GameOver) {
    uint8_t maxCombo = 0;
    uint16_t linesSent = 0;
    uint16_t linesRecieved = 0;
    uint16_t linesBlocked = 0;
    uint16_t bpm = 0;
};

PACKET(NP_Winner) {
    uint8_t maxCombo = 0;
    uint16_t linesSent = 0;
    uint16_t linesRecieved = 0;
    uint16_t linesBlocked = 0;
    uint16_t bpm = 0;
    uint32_t duration = 0;
    uint16_t pieceCount = 0;
};

PACKET(NP_ChatMsg) {
    std::string target;
    std::string message;
};

PACKET(NP_CreateRoom) {
    std::string name;
    uint8_t maxPlayers = 0;
};

PACKET(NP_CreateTournament) {
    std::string name;
    uint8_t sets = 0;
    uint8_t rounds = 0;
};

PACKET(NP_ConfirmUDP){};

PACKET(NP_Gamestate){};

PACKET(NP_Ping){};

PACKET(NP_JoinRoom) { uint16_t roomID = 0; };

PACKET(NP_Leave){};

PACKET(NP_LinesSent) { uint16_t amount = 0; };

PACKET(NP_GarbageCleared) { uint16_t amount = 0; };

PACKET(NP_LinesBlocked) { uint16_t amount = 0; };

PACKET(NP_Away){};

PACKET(NP_NotAway){};

PACKET(NP_Ready){};

PACKET(NP_NotReady){};

PACKET(NP_TournamentJoin) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentSignUp) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentWithdraw) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentCloseSignUp) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentStart) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentJoinGame) {
    uint16_t tournamentID = 0;
    uint16_t gameID = 0;
};

PACKET(NP_TournamentLeftPanel) { uint16_t tournamentID = 0; };

PACKET(NP_TournamentListRefresh){};

PACKET(NP_RoomListRefresh){};

PACKET(NP_ChallengePlay) { uint16_t challengeID = 0; };

PACKET(NP_ChallengeWatchReplay) {
    uint16_t challengeID = 0;
    uint16_t replayID = 0;
};

PACKET(NP_SpectatorJoin) { uint16_t roomID = 0; };

PACKET(NP_SpectatorLeave) { uint16_t roomID = 0; };

PACKET(NP_MatchmakingJoin){};

PACKET(NP_MatchmakingLeave){};

PACKET(NP_HandicapSet) {
    uint8_t handicap = 0;  // in %
};

    // Server outgoing

#endif  // NETWORKPACKETS_HPP