#include <iostream>
#include <vector>
#include <map>
#include <cmath> 
using namespace std;

class HMM {
    // characteristics of HMM
    int N; // number of states
    vector<long double> initial_prob;
    vector<string> states;
    vector<vector<long double>> transition_prob;
    vector<char> emissions;
    vector<map<char,long double>> emission_prob;

    public:
    void input_hmm();
    void do_viterbi(string& obs);
};

void HMM::input_hmm() {
    int n;
    // cout<<"Enter number of states: ";
    cin>>n;
    N = n;

    // cout<<"Enter state names: \n";
    for(int i=0;i<n;i++) {
        string el;
        cin>>el;
        states.push_back(el);
    } 

    // cout<<"Enter initial probabilities of states (in order): \n";
    for(int i=0;i<n;i++) {
        long double el;
        cin>>el;
        initial_prob.push_back(el);
    }

    // cout<<"Enter transition probabilities matrix (in order): \n";
    for(int i=0;i<n;i++) {
        vector<long double> v(n);
        for(int j=0;j<n;j++) {
            cin>>v[j];
        }
        transition_prob.push_back(v);
    }

    // cout<<"Enter emission alphabet: \n";
    for(int i=0;i<n;i++) {
        char el;
        cin>>el;
        emissions.push_back(el);
    }

    for(int i=0;i<n;i++) {
        // cout<<"Enter emission probabilities for state: "<<i+1<<" (in order of emission alphabet)"<<"\n";
        map<char,long double> emi_p;
        for(int j=0;j<emissions.size();j++) {
            long double el;
            cin>>el;
            emi_p[emissions[j]]=el;
        }
        emission_prob.push_back(emi_p);
    }
}

void HMM::do_viterbi(string& obs) {
    int T = obs.length();
    vector<vector<int>> backpointers;
    vector<vector<long double>> viterbi;

    for (int i=0;i<N;i++) {
        vector<long double> x(T,0);
        vector<int> y(T,0);
        viterbi.push_back(x);
        backpointers.push_back(y);
    }

    //initialisation step
    for(int i=0;i<N;i++) {
        viterbi[i][0] = initial_prob[i]*emission_prob[i][obs[0]];
    }

    for(int i=1;i<T;i++) {
        for(int j=0;j<N;j++) {
            //find max probability
            long double max=-1;
            long double argmax=-1;
            
            for (int k=0;k<N;k++) {
                long double curr=0;
                curr = viterbi[k][i-1]*transition_prob[k][j]*emission_prob[j][obs[i]];
                
                if (curr>max) {
                    max = curr;
                    argmax = k+1;
                }
            }

            viterbi[j][i] = max;
            backpointers[j][i] = argmax;
        }
    }

    long double bestPathProb=0;
    long double bestPathState;
    for (int i=0;i<N;i++) {
        if (viterbi[i][T-1]>bestPathProb) {
            bestPathProb = viterbi[i][T-1];
            bestPathState=i+1;
        }
    }

    vector<string> bestPath(T);
    for (int i=0;i<T;i++) {
        bestPath[T-i-1]=states[bestPathState-1];
        bestPathState = backpointers[bestPathState-1][T-i-1];
    }

    cout<<"Best Path Probability: "<<bestPathProb<<"\n";
    cout<<"Log of probability: "<<log(bestPathProb)<<"\n";
    cout<<"Best Path: \n";
    for (int i=0;i<T;i++) {
        cout<<bestPath[i]<<" ";
    }
    cout<<"\n";
}

int main() {
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif

    string observations;
    // cout<<"Enter Observed sequence: \n";
    cin>>observations;

    HMM h;
    h.input_hmm();
    h.do_viterbi(observations);

    return 0;
}