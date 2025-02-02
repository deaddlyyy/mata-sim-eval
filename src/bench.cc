/**
 * NOTE: Input automata, that are of type `NFA-bits` are mintermized!
 *  - If you want to skip mintermization, set the variable `MINTERMIZE_AUTOMATA` below to `false`
 */

#include "mata/nfa/algorithms.hh"
#include "utils/utils.hh"
#include "mata/nfa/nfa.hh"
#include "mata/nfa/learning.hh"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

using namespace mata::nfa;

const bool MINTERMIZE_AUTOMATA = false;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Something is missing\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
    std::string algorithm = argv[2];

    Nfa aut;
    mata::OnTheFlyAlphabet alphabet{};
    if (load_automaton(filename, aut, alphabet, MINTERMIZE_AUTOMATA) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Setting precision of the times to fixed points and 4 decimal places
    std::cout << std::fixed << std::setprecision(4);
    Nfa conjecture;

    if(algorithm == "lstar"){
        TIME_BEGIN(Lstar);
        conjecture = learn(aut, ParameterMap{{ "algorithm", "lstar"}});
        TIME_END(Lstar);
    } else if(algorithm == "brzozowski"){
        TIME_BEGIN(Brzozowski);
        conjecture = minimize_brzozowski(aut);
        TIME_END(Brzozowski);
    }
    else if(algorithm == "nlstar"){
        TIME_BEGIN(NLstar);
        conjecture = learn(aut, ParameterMap{{ "algorithm", "nlstar"}});
        TIME_END(NLstar);
    }
    else if(algorithm == "residuals"){
        TIME_BEGIN(residuals);
        StateRenaming state_renaming;
        ParameterMap params_after;
        params_after["algorithm"] = "residual";
        params_after["type"] = "with";
        params_after["direction"] = "forward";
        conjecture = reduce(aut, &state_renaming, params_after);
        TIME_END(residuals);
    }
    return EXIT_SUCCESS;
}
