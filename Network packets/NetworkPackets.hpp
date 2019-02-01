#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "NetworkPacketHandler.hpp"

namespace sf {
class Packet;
}

using PM = os::PacketManager<Stream::To<sf::Packet>>;

// Client outgoing

PACKET(NP_Replay){};

PACKET(NP_LoginRequest) {
    uint16_t clientVersion = 0;
    uint8_t guest = 0;
    std::string nameOrHash;
};

struct RoundScore {
    uint8_t max_combo;
    uint16_t lines_sent;
    uint16_t lines_recieved;
    uint16_t lines_blocked;
    uint16_t bpm;
};

PACKET(NP_GameOver) { RoundScore score; };

PACKET(NP_Winner) {
    RoundScore score;
    uint32_t duration;
    uint16_t piece_count;
};

PACKET(NP_ChatMsg) {
    std::string sender;
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

PACKET(NP_ConfirmUdp){};

PACKET(NP_Gamestate){};

PACKET(NP_Ping){};

PACKET(NP_JoinRoom) { uint16_t roomID = 0; };

PACKET(NP_LinesSent) { uint16_t amount = 0; };

PACKET(NP_GarbageCleared) { uint16_t amount = 0; };

PACKET(NP_LinesBlocked) { uint16_t amount = 0; };

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

PACKET(NP_ChallengePlay) { uint16_t challengeID = 0; };

PACKET(NP_ChallengeWatchReplay) {
    uint16_t challengeID = 0;
    uint16_t replayID = 0;
};

PACKET(NP_SpectatorJoin) { uint16_t roomID = 0; };

PACKET(NP_SpectatorLeave) { uint16_t roomID = 0; };

PACKET(NP_HandicapSet) {
    uint8_t handicap = 0;  // in %
};

SIGNAL(NP_Leave);
SIGNAL(NP_Away);
SIGNAL(NP_NotAway);
SIGNAL(NP_Ready);
SIGNAL(NP_NotReady);
SIGNAL(NP_TournamentListRefresh);
SIGNAL(NP_RoomListRefresh);
SIGNAL(NP_MatchmakingJoin);
SIGNAL(NP_MatchmakingLeave);

// Server outgoing

struct RoomInfo {
    uint16_t id;
    std::string name;
    uint8_t currentPlayers;
    uint8_t maxPlayers;
};

struct ClientInfo {
    uint16_t id;
    std::string name;
};

PACKET(NP_Welcome) {
    uint16_t yourID;
    std::string message;
    std::vector<RoomInfo> rooms;
    uint16_t matchmakingQueue;
    uint16_t matchmakingPlaying;
    uint16_t clientCount;
    std::vector<ClientInfo> clients;
};

PACKET(NP_ReplaySend){};

struct ChallengeInfo {
    uint16_t id;
    std::string name;
    std::string label;
};

PACKET(NP_ChallengeList) { std::vector<ChallengeInfo> challenges; };

PACKET(NP_JoinResponse) {
    uint16_t status;
    uint16_t seed1, seed2;
    std::vector<ClientInfo> players;
};

PACKET(NP_ClientJoinedRoom) { ClientInfo player; };

struct ListEntryTitle {
    uint16_t x_pos;
    std::string text;
};

struct ListEntry {
    std::string text;
};

struct ChallengeScore {
    std::string name;
    std::vector<ListEntry> scores;
};

PACKET(NP_ChallengeLeaderboard) {
    uint16_t id;
    std::vector<ListEntryTitle> columns;
    std::vector<ChallengeScore> scores;
};

PACKET(NP_ReplayRequest) { std::string message; };

PACKET(NP_ChallengeNotRecord) { std::string message; };

struct RoundScoreServer {
    RoundScore score;
    uint16_t id;
    uint8_t ffa_rank;
    uint8_t position;
    uint16_t game_score;
    float lines_adjusted;
    uint16_t ffa_points;
};

PACKET(NP_RoomScore) {
    uint16_t round_lenght;
    std::vector<RoundScoreServer> scores;
};

PACKET(NP_AuthResult) {
    uint8_t status;
    uint16_t id;
    std::string name;
};

PACKET(NP_Alert) { std::string text; };

PACKET(NP_RoomList) { std::vector<RoomInfo> rooms; };

PACKET(NP_MatchMaking) {
    uint16_t queue;
    uint16_t playing;
};

PACKET(NP_ClientJoinedServer) { ClientInfo player; };

PACKET(NP_ClientLeftServer) { uint16_t id; };

struct TournamentInfo {
    uint16_t id;
    std::string name;
    uint8_t status;
    uint8_t players;
};

PACKET(NP_TournamentList) { std::vector<TournamentInfo> tournaments; };

struct TournamentScore {
    uint8_t sets;
    std::vector<uint8_t> rounds;
};

struct TournamentGame {
    uint16_t id;
    uint8_t depth;
    uint8_t status;
    std::vector<ClientInfo> players;
    std::vector<TournamentScore> scores;
};

PACKET(NP_TournamentInfo) {
    uint16_t id;
    uint8_t grade;
    uint8_t rounds;
    uint8_t sets;
    uint64_t start_time;
    std::vector<ClientInfo> players;
    std::vector<uint16_t> moderators;
    uint8_t status;
    std::vector<TournamentGame> games;
};

struct GameScore {
    uint16_t id;
    uint8_t sets;
    uint8_t rounds;
    uint8_t status;
};

PACKET(NP_GameScore) { std::vector<GameScore> scores; };

PACKET(NP_TournamentGames) {
    uint16_t id;
    std::vector<TournamentGame> games;
};

PACKET(NP_PingReply) { uint16_t ping; };

PACKET(NP_TournamentGameReady) {
    uint16_t tournament_id;
    uint16_t game_id;
};

PACKET(NP_CountdownStart) {
    uint16_t seed1;
    uint16_t seed2;
};

PACKET(NP_CountdownStop) { uint8_t status; };

PACKET(NP_ClientLeftRoom) { uint16_t id; };

PACKET(NP_RoundStart) {
    uint16_t seed1;
    uint16_t seed2;
};

PACKET(NP_ClientAway) { uint16_t id; };

PACKET(NP_ClientNotAway) { uint16_t id; };

PACKET(NP_PlayerPosition) {
    uint16_t id;
    uint16_t position;
};

PACKET(NP_ClientReady) { uint16_t id; };

PACKET(NP_ClientNotReady) { uint16_t id; };

PACKET(NP_Kick) { std::string message; };

PACKET(NP_AvgBpm) {
    uint16_t id;
    uint16_t average_bpm;
};

PACKET(NP_TournamentTimeWo) { uint16_t time; };

SIGNAL(NP_TournamentNotEnough);
SIGNAL(NP_NotAsGuest);
SIGNAL(NP_RoundEnd);
SIGNAL(NP_YouWon);
SIGNAL(NP_UdpConfirmed);
SIGNAL(NP_NotHero);
SIGNAL(NP_ClientJoinedMatchmaking);
SIGNAL(NP_ClientLeftMatchmaking);
SIGNAL(NP_ClientRemovedMatchmaking);
SIGNAL(NP_ClientStillInMatchmaking);

#endif  // NETWORKPACKETS_HPP