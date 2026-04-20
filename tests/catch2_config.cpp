#define CATCH_CONFIG_DEFAULT_REPORTER "console"

#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]) {
    Catch::Session session;
    session.configData().rngSeed = 1;
    return session.run(argc, argv);
}

