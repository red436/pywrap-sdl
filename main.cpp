#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <iterator>
#include "SDL.h"
#include <SDL2/SDL_image.h>
#include <box2d.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
// #include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_gfx>

namespace py = pybind11;
using namespace std;

class Window {

    private:
 
    map<string, SDL_Surface *> surfaces; 
    map<string, SDL_Rect> rects;
    // map<string, Mix_Chunk *> chunks;
    // map<string, Mix_Music *> tracks;

    public:

    
    SDL_Window *screen;
    SDL_Surface *gScreen;
    SDL_AudioDeviceID device_id;
    SDL_Event event;
    SDL_Rect scrRect;

    void updateWindow()
    {
        SDL_UpdateWindowSurface( screen );
    }
    unsigned int getTicks()
    {
        return (SDL_GetTicks());
    }
    void clearWindow()
    {
        SDL_FillRect(gScreen, NULL, SDL_MapRGB(gScreen->format, 0, 0, 0));
    }
    //void addChunk( string name )
    //{
    //    chunks[name] = Mix_LoadWAV(name.c_str());
    //}
    //void playRawSound( const char *bytes, int length )
    //{
        // cout << to_string(bytes.itemsize) << endl;
        // cout << bytes << endl;
    //    unsigned char *ubytes = (unsigned char *)bytes;
    //    Mix_Chunk * chunk = Mix_QuickLoad_RAW(ubytes, length);
    //    if(!chunk)
    //    {
    //        cout << "Failed to load RAW chunk " << SDL_GetError() << endl;
    //    }
    //    if( Mix_PlayChannel( -1, chunk, 0) == -1)
    //    {
    //        cout << "Failed to play RAW chunk " << SDL_GetError() << endl;
    //    }
    //}
    void playRawSound( const int16_t byte )
    {
        // cout << bytes << " ";
        // cout << to_string(bytes.itemsize) << endl;
        // cout << bytes << endl;
        // istringstream ss (hex_chars);
        //  vector<unsigned char> bytes;
        // unsigned int c;
        // while(ss >> hex >> c)
        // {
        //     bytes.push_back(c);
        // }
        // unsigned char *ubytes = (unsigned char *)bytes;
        // for(int i; i < bytes.size(); i++)
        // {
        //     ubytes[i] = bytes[i];
        // }
        //Mix_Chunk * chunk = Mix_QuickLoad_RAW(ubytes, length);
        //if(!chunk)
        //{
        //    cout << "Failed to load RAW chunk " << SDL_GetError() << endl;
        //}
        //if( Mix_PlayChannel( -1, chunk, 0) == -1)
        //{
        //    cout << "Failed to play RAW chunk " << SDL_GetError() << endl;
        //}
        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(device_id, &byte, sample_size);
    }

    void addSurface( string name )
    {
        SDL_Surface *surf;
        SDL_Surface *dest;
        surf = IMG_Load(name.c_str());
        if(!surf)
        {
            cout << "Image load error:" << IMG_GetError() << endl;
        }
        dest = SDL_ConvertSurface( surf, gScreen->format, 0);
        surfaces[name] = dest;
        SDL_FreeSurface(surf);
    }
    void addRect(string name)
    {
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = 0;
        rect.h = 0;
        rects[name] = rect;
    }
    void applySurface( string surfaceName, double x, double y )
    {
        
        SDL_Rect rect;
        rect.w = surfaces[surfaceName]->w;
        rect.h = surfaces[surfaceName]->h;
        rect.x = x;
        rect.y = y;
        SDL_BlitSurface(surfaces[surfaceName], NULL, gScreen, &rect);
    }
    void makeWindow(int w, int h, string name)
    {
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        {
            cout << "Could not init SDL." << endl;
            exit(1);
        }
        SDL_AudioSpec  audio_spec;
        SDL_zero(audio_spec);
        audio_spec.freq = 44100;
        audio_spec.format = AUDIO_S16SYS;
        audio_spec.channels = 1;
        audio_spec.samples = 1024;
        audio_spec.callback = NULL;
        device_id = SDL_OpenAudioDevice( NULL, 0, &audio_spec, NULL, 0 );
        if( device_id == -1 )
        {
            cout << "Could not init mixer." << endl;
            exit(0);
        }
        // Mix_Volume(-1, MIX_MAX_VOLUME / 2);
        screen = SDL_CreateWindow(name.c_str(), 0, 0, w, h, 0);
        gScreen = SDL_GetWindowSurface( screen );
        scrRect.w = gScreen->w;
        scrRect.h = gScreen->h;
        scrRect.x = 0;
        scrRect.y = 0;
        IMG_Init(IMG_INIT_PNG);
        // Mix_Init(MIX_INIT_MP3);
        SDL_PauseAudioDevice(device_id, 0);
    }
    py::dict mouseXY()
    {
        int x, y;
        py::dict result;
        SDL_GetMouseState(&x, &y);
        result["x"] = x;
        result["y"] = y;
        return result;
    }
    py::dict getEvent() 
    {
        py::dict result;
        result["type"] = "";
        result["key"] = "";
        result["state"] = "";
        result["button"] = "";
        result["x"] = "";
        result["y"] = "";
        if(SDL_PollEvent(&event))
        {

		switch( event.type )
		{
		    case SDL_KEYUP:
			result["type"] = "KEYBOARD";
			result["state"] = "UP";
			switch( event.key.keysym.sym )
			{
			    case SDLK_r:
			       result["key"] = "KEY_r";
			       break;
			    case SDLK_q:
			       result["key"] = "KEY_q";
			       break;
			    case SDLK_LEFT:
			       result["key"] = "KEY_LEFT";
			       break;
			    case SDLK_RIGHT:
			       result["key"] = "KEY_RIGHT";
			       break;
			    case SDLK_UP:
			       result["key"] = "KEY_UP";
			       break;
			    case SDLK_DOWN:
			       result["key"] = "KEY_DOWN";
			       break;
			    default:
			       break;
			}
			break;
		    case SDL_KEYDOWN:
			result["type"] = "KEYBOARD";
			result["state"] = "DOWN";
			switch( event.key.keysym.sym )
			{
			    case SDLK_r:
			       result["key"] = "KEY_r";
			       break;
			    case SDLK_q:
			       result["key"] = "KEY_q";
			       break;
			    case SDLK_LEFT:
			       result["key"] = "KEY_LEFT";
			       break;
			    case SDLK_RIGHT:
			       result["key"] = "KEY_RIGHT";
			       break;
			    case SDLK_UP:
			       result["key"] = "KEY_UP";
			       break;
			    case SDLK_DOWN:
			       result["key"] = "KEY_DOWN";
			       break;
			    default:
			       break;
			}
			break;
		    case SDL_MOUSEBUTTONDOWN:
			result["type"] = "MOUSE";
			result["state"] = "DOWN";
			switch( event.button.button )
			{
			    case SDL_BUTTON_LEFT:
			       result["button"] = "BUTTON_LEFT";
			       result["x"] = event.button.x;
			       result["y"] = event.button.y;
			       break;
			    case SDL_BUTTON_RIGHT:
			       result["button"] = "BUTTON_RIGHT";
			       result["x"] = event.button.x;
			       result["y"] = event.button.y;
			       break;
			    default:
			       break;
			}
			break;
		    case SDL_MOUSEBUTTONUP:
			result["type"] = "MOUSE";
			result["state"] = "UP";
			switch( event.button.button )
			{
			   case SDL_BUTTON_LEFT:
			       result["button"] = "BUTTON_LEFT";
			       result["x"] = event.button.x;
			       result["y"] = event.button.y;
			       break;
			   case SDL_BUTTON_RIGHT:
			       result["button"] = "BUTTON_RIGHT";
			       result["x"] = event.button.x;
			       result["y"] = event.button.y;
			       break;
			   default:
			       break;
			}
			break;
		    case SDL_QUIT:
			result["type"] = "QUIT";
			break;
		    default:
			break;
		}
        }
        else
        {
            result["type"] = "NO_EVENT";
        }
        return result;
    }
    void closeWindow()
    {
        SDL_FreeSurface( gScreen );
        for( const auto& [key, value] : surfaces)
        {
            SDL_FreeSurface(value);
        }
        SDL_DestroyWindow( screen );
        SDL_CloseAudioDevice( device_id );
        SDL_Quit();
        IMG_Quit();
        // Mix_CloseAudio();
        // Mix_Quit();
    }
};

PYBIND11_EMBEDDED_MODULE(Window, m)
{
    m.doc() = "SDL Window binding";

    py::class_<Window>(m, "Window")
    .def(py::init<>())
    .def("makeWindow", &Window::makeWindow)
    .def("playRawSound", &Window::playRawSound)
    .def("getEvent", &Window::getEvent)
    .def("getTicks", &Window::getTicks)
    .def("mouseXY", &Window::mouseXY)
    .def("addRect", &Window::addRect)
    .def("addSurface", &Window::addSurface)
    .def("applySurface", &Window::applySurface)
    .def("clearWindow", &Window::clearWindow)
    .def("updateWindow", &Window::updateWindow)
    .def("closeWindow", &Window::closeWindow);
}

// PYBIND11_EMBEDDED_MODULE(Event, m)
// {
//    m.doc() = "SDL event sturcture";
//    py::class_<Event>(m, "Event")
//    .def(py::init<>())
//    .def_readwrite("events", &Event::events);
// }

class __attribute__((visibility("default"))) Interpreter {

    py::object scope;

    public:

    void init()
    {
       py::initialize_interpreter();
       scope = py::module_::import("__main__").attr("__dict__");
    }
 
    void run(std::string name)
    {
       py::eval_file(name, scope);
    }
   
    void finalize()
    {
       py::finalize_interpreter();
    }
};

int main()
{
    bool reload = false;
    bool quit = false;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    string buffer;
    int addresslen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
       cout << "Failed to init sockets." << endl;
       exit(1);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
       cout << "Set socket options failed." << endl;
       exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(4000);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
       cout << "Failed to bind address." << endl;
       exit(1);
    }

    if(listen(server_fd, 3) < 0)
    {
       cout << "Failed to listen to port." << endl;
       exit(1);
    }

    std::string input;
    Interpreter interpreter;
    interpreter.init();
    interpreter.run("main.py");
    while(quit == false)
    {
       char res[1024];

       if(( new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addresslen)) < 0)
       {
          cout << "Failed to accept incomming conections." << endl;
          exit(1);
       }

       valread = read( new_socket, res, 1024);
       string buffer( res );
       if(buffer.find("restart") != string::npos)
       {
          interpreter.finalize();
          interpreter.init();
          interpreter.run("main.py");
       }
       if(buffer.find("quit") != string::npos)
       {
          exit(0);
       }
    }
    return 0;
}
