#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "NetworkPacketHandler.hpp"

// Client outgoing

PACKET(NP_Replay){};

PACKET(NP_LoginRequest) {
    uint16_t clientVersion;
    uint8_t guest;
    std::string nameOrHash;
};

PACKET(NP_GameOver) {
    uint8_t maxCombo;
    uint16_t linesSent;
    uint16_t linesRecieved;
    uint16_t linesBlocked;
    uint16_t bpm;
};

PACKET(NP_Winner) {
    uint8_t maxCombo;
    uint16_t linesSent;
    uint16_t linesRecieved;
    uint16_t linesBlocked;
    uint16_t bpm;
    uint32_t duration;
    uint16_t pieceCount;
};

PACKET(NP_ChatMsg) {
    std::string target;
    std::string message;
};

PACKET(NP_CreateRoom) {
    std::string name;
    uint8_t maxPlayers;
};

PACKET(NP_CreateTournament) {
    std::string name;
    uint8_t sets;
    uint8_t rounds;
};

PACKET(NP_ConfirmUDP){};

PACKET(NP_Gamestate){};

PACKET(NP_Ping){};

PACKET(NP_JoinRoom) { uint16_t roomID; };

PACKET(NP_Leave){};

PACKET(NP_LinesSent) { uint16_t amount; };

PACKET(NP_GarbageCleared) { uint16_t amount; };

PACKET(NP_LinesBlocked) { uint16_t amount; };

PACKET(NP_Away){};

PACKET(NP_NotAway){};

PACKET(NP_Ready){};

PACKET(NP_NotReady){};

PACKET(NP_TournamentJoin) { uint16_t tournamentID; }

PACKET(NP_TournamentSignUp) { uint16_t tournamentID; };

PACKET(NP_TournamentWithdraw) { uint16_t tournamentID; };

PACKET(NP_TournamentCloseSignUp) { uint16_t tournamentID; };

PACKET(NP_TournamentStart) { uint16_t tournamentID; };

PACKET(NP_TournamentJoinGame) {
    uint16_t tournamentID;
    uint16_t gameID;
};

PACKET(NP_TournamentLeftPanel) { uint16_t tournamentID; };

PACKET(NP_TournamentListRefresh){};

PACKET(NP_RoomListRefresh){};

PACKET(NP_ChallengePlay) { uint16_t challengeID; };

PACKET(NP_ChallengeWatchReplay) {
    uint16_t challengeID;
    uint16_t replayID;
};

PACKET(NP_SpectatorJoin) { uint16_t roomID; };

PACKET(NP_SpectatorLeave) { uint16_t roomID; };

PACKET(NP_MatchmakingJoin){};

PACKET(NP_MatchmakingLeave){};

PACKET(NP_HandicapSet) {
    uint8_t handicap;  // in %
};

    // Server outgoing

#endif  // NETWORKPACKETS_HPP