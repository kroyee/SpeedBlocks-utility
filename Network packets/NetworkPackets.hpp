#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "../GUI serialize/GuiSerialize.hpp"
#include "NetworkPacketHandler.hpp"

namespace sf {
class Packet;
}

using PM = os::PacketManager<Stream::To<sf::Packet>>;

// Client outgoing

PACKET(NP_Replay) {
    uint16_t type = 0;
    uint16_t id = 0;
    std::string name;
    std::vector<uint8_t> data;
};

PACKET(NP_LoginRequest) {
    uint16_t clientVersion = 0;
    uint8_t guest = 0;
    std::string nameOrHash;
};

struct RoundScore {
    uint8_t max_combo = 0;
    uint16_t lines_sent = 0;
    uint16_t lines_recieved = 0;
    uint16_t lines_blocked = 0;
    uint16_t bpm = 0;
};

PACKET(NP_GameOver) {
    RoundScore score;
    uint32_t duration = 0;
    uint16_t piece_count = 0;
};

PACKET(NP_Winner) {
    RoundScore score;
    uint32_t duration = 0;
    uint16_t piece_count = 0;
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

PACKET(NP_ConfirmUdp) { uint16_t id = 0; };

PACKET(NP_Gamestate) {
    uint16_t id = 0;
    uint8_t count = 0;
    std::vector<uint8_t> data;
};

PACKET(NP_Ping) {
    uint16_t client_id = 0;
    uint8_t ping_id = 0;
};

PACKET(NP_JoinRoom) { uint16_t roomID = 0; };

PACKET(NP_LinesSent) { uint8_t amount = 0; };

PACKET(NP_GarbageCleared) { uint8_t amount = 0; };

PACKET(NP_LinesBlocked) { uint8_t amount = 0; };

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

PACKET(NP_SpectatorJoin) {
    uint16_t tournamentID = 0;
    uint16_t roomID = 0;
};

PACKET(NP_HandicapSet) {
    uint8_t handicap = 0;  // in %
};

SIGNAL(NP_SpectatorLeave);
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
    uint16_t id = 0;
    std::string name;
    uint8_t currentPlayers = 0;
    uint8_t maxPlayers = 0;
};

struct ClientInfo {
    uint16_t id = 0;
    std::string name;
};

PACKET(NP_Welcome) {
    uint16_t yourID = 0;
    std::string message;
};

PACKET(NP_ClientList) { std::vector<ClientInfo> clients; };

PACKET(NP_Matchmaking) {
    uint16_t matchmakingQueue = 0;
    uint16_t matchmakingPlaying = 0;
};

PACKET(NP_ReplaySend){};

struct ChallengeInfo {
    uint16_t id = 0;
    std::string name;
    std::string label;
};

PACKET(NP_ChallengeList) { std::vector<ChallengeInfo> challenges; };

PACKET(NP_JoinResponse) {
    uint16_t status = 0;
    uint16_t seed1 = 0, seed2 = 0;
    std::vector<ClientInfo> players;
};

PACKET(NP_ClientJoinedRoom) { ClientInfo player; };

struct ListEntryTitle {
    uint16_t x_pos = 0;
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
    uint16_t id = 0;
    std::vector<ListEntryTitle> columns;
    std::vector<ChallengeScore> scores;
};

PACKET(NP_JoinChallenge) { uint16_t id = 0; };

PACKET(NP_ReplayRequest) { std::string message; };

PACKET(NP_ChallengeNotRecord) { std::string message; };

struct RoundScoreServer {
    RoundScore score;
    uint16_t id = 0;
    uint8_t ffa_rank = 0;
    uint8_t position = 0;
    uint16_t game_score = 0;
    float lines_adjusted = 0;
    uint16_t ffa_points = 0;
};

PACKET(NP_RoomScore) {
    uint16_t round_lenght = 0;
    std::vector<RoundScoreServer> scores;
};

PACKET(NP_AuthResult) {
    uint8_t status = 0;
    uint16_t id = 0;
    std::string name;
};

PACKET(NP_Alert) { std::string text; };

PACKET(NP_RoomList) { std::vector<RoomInfo> rooms; };

PACKET(NP_RoomAdd) { RoomInfo room; };

PACKET(NP_RoomRemove) { uint16_t id = 0; };

PACKET(NP_MatchMaking) {
    uint16_t queue = 0;
    uint16_t playing = 0;
};

PACKET(NP_ClientJoinedServer) { ClientInfo player; };

PACKET(NP_ClientLeftServer) { uint16_t id = 0; };

struct TournamentInfo {
    uint16_t id = 0;
    std::string name;
    uint8_t status = 0;
    uint16_t players = 0;
};

PACKET(NP_TournamentList) { std::vector<TournamentInfo> tournaments; };

struct TournamentScore {
    uint8_t sets = 0;
    std::vector<uint8_t> rounds;
};

struct TournamentGame {
    uint16_t id = 0;
    uint8_t depth = 0;
    uint8_t status = 0;
    std::vector<ClientInfo> players;
    std::vector<TournamentScore> scores;
};

PACKET(NP_TournamentInfo) {
    uint16_t id = 0;
    uint8_t grade = 0;
    uint8_t rounds = 0;
    uint8_t sets = 0;
    uint64_t start_time = 0;
    std::vector<ClientInfo> players;
    std::vector<uint16_t> moderators;
    uint8_t status = 0;
    std::vector<TournamentGame> games;
};

struct GameScore {
    uint16_t id = 0;
    uint8_t sets = 0;
    uint8_t rounds = 0;
    uint8_t status = 0;
};

PACKET(NP_GameScore) { std::vector<GameScore> scores; };

PACKET(NP_TournamentGames) {
    uint16_t id = 0;
    std::vector<TournamentGame> games;
};

PACKET(NP_PingReply) { uint16_t ping = 0; };

PACKET(NP_TournamentGameReady) {
    uint16_t tournament_id = 0;
    uint16_t game_id = 0;
};

PACKET(NP_CountdownStart) {
    uint16_t seed1 = 0;
    uint16_t seed2 = 0;
    uint8_t status = 0;
};

PACKET(NP_CountdownStop) { uint8_t status = 0; };

PACKET(NP_Countdown) { uint16_t countdown = 0; };

PACKET(NP_ClientLeftRoom) { uint16_t id = 0; };

PACKET(NP_RoundStart) {
    uint16_t seed1 = 0;
    uint16_t seed2 = 0;
};

PACKET(NP_ClientAway) { uint16_t id = 0; };

PACKET(NP_ClientNotAway) { uint16_t id = 0; };

PACKET(NP_PlayerPosition) {
    uint16_t id = 0;
    uint8_t position = 0;
};

PACKET(NP_ClientReady) { uint16_t id = 0; };

PACKET(NP_ClientNotReady) { uint16_t id = 0; };

PACKET(NP_Kick) { std::string message; };

PACKET(NP_AvgBpm) {
    uint16_t id = 0;
    uint16_t average_bpm = 0;
};

PACKET(NP_TournamentTimeWo) { uint16_t time = 0; };

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