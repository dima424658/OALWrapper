#include "OALWrapper/OAL_Funcs.h"
#include "OALWrapper/OAL_Stream.h"

#include <string>
#include <cstdlib>
#include <list>
#include <thread>

#include <signal.h>

using namespace std;

typedef list<string> stringList;
typedef stringList::iterator stringListIt;

class Playlist {
    stringList      mSongs;
    stringListIt    mCurrentSong;

    cOAL_Stream*    mStream;
    int             mSource;

    bool            mThreadAlive;
    std::thread     mUpdaterThread;

public:
    Playlist(const stringList& aSongs) :
                                        mStream(0),
                                        mSource(0),
                                        mThreadAlive(false)
    {
        mSongs.assign(aSongs.begin(), aSongs.end());
        mCurrentSong = mSongs.begin();
        Init();
    }
    Playlist(int count, const char *const aSongs[]) : mStream(0), mSource(0)
    {
        for (int i = 0; i < count; ++i) {
            mSongs.push_back(aSongs[i]);
        }
        mCurrentSong = mSongs.begin();
        Init();
    }
    
    ~Playlist()
    {
        stopSong();
        if (mThreadAlive) {
            mThreadAlive = false;
            if(mUpdaterThread.joinable())
                mUpdaterThread.join();
        }
    }
    
    void Init()
    {
        mThreadAlive = true;
        mUpdaterThread = std::thread{Playlist::UpdaterThread, std::ref(*this)};
    }
    
    void playSong()
    {
        stopSong();
        printf("Loading stream \"%s\" ... ",(*mCurrentSong).c_str());

        mStream = OAL_Stream_Load ((*mCurrentSong));

        printf("%s\n", mStream ? "Success" : "Failed");
        
        if (mStream) {
            printf("\tChannels : %d\n\tFrequency : %d\n", mStream->GetChannels(), mStream->GetFrequency() );

            mSource = OAL_Stream_Play(OAL_FREE, mStream, 1.0f, false);
        }
    }

    static void UpdaterThread(Playlist& playlist)
    {
        int lWaitTime = 1000 / 30; // 30times /second
        while (playlist.mThreadAlive) {
            if (playlist.mSource) {
                if (!OAL_Source_IsPlaying(playlist.mSource)) {
                    ++playlist.mCurrentSong;
                    if (playlist.mCurrentSong == playlist.mSongs.end()) {
                        playlist.mCurrentSong = playlist.mSongs.begin();
                    }
                    playlist.playSong();
                }
            }
		    std::this_thread::sleep_for(std::chrono::milliseconds{lWaitTime});
        }
    }
    
    void stopSong()
    {
        if (mSource) {
            OAL_Source_Stop(mSource);
            mSource = 0;
        }

        if (mStream) {
            OAL_Stream_Unload(mStream);
            mStream = 0;
        }
    }
};

static bool done = false;

void sighandler(int sig)
{
    done = true;
}

int main (int argc, const char *const argv[])
{
	if ( argc <= 1 )
	{
		printf ("Usage : %s \"sample1.ogg\" [\"sample2.ogg\" \"sample3.ogg\" ...]\n", argv[0]);
        printf ("\tThe application will simply loop through all of the files\n\n");
        return 1;
	}

    Playlist* lPlaylist = new Playlist(argc - 1, &argv[1]);

    printf ("Initializing OpenAL... ");
    fflush(stdout);
    OAL_SetupLogging(true,eOAL_LogOutput_File,eOAL_LogVerbose_High);
    
    cOAL_Init_Params oal_parms;
    oal_parms.mlStreamingBufferSize = 8192;

    if (OAL_Init(oal_parms)==false)
    {
        printf ("Failed - Check your OpenAL installation\n");
        return 1;
    }
    else
        printf ("Success\n");

    lPlaylist->playSong();
    
    signal(SIGINT, sighandler);
    
    while (!done) {
		std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }

    printf ("Cleaning up...\n");

    delete lPlaylist;
    lPlaylist = 0;

    OAL_Close();

	return 0;
}

