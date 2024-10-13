//https://zhuanlan.zhihu.com/p/339880005
#include <list>
#include <thread>
#include <condition_variable>
#include "RenderCommandList.h"
#include "Window.h"
#include "Engine.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

std::condition_variable cv;
std::mutex cv_m; // This mutex is used for three purposes:
// 1) to synchronize accesses to i
// 2) to synchronize accesses to std::cerr
// 3) for the condition variable cv
int i = 0;

void waits()
{
	std::unique_lock<std::mutex> lk(cv_m);
	std::cerr << "Waiting... \n";
	cv.wait(lk, [] { return i == 1; });
	std::cerr << "...finished waiting. i == 1\n";
}

void signals()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	{
		std::lock_guard<std::mutex> lk(cv_m);
		std::cerr << "Notifying...\n";
	}
	cv.notify_all();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	{
		std::lock_guard<std::mutex> lk(cv_m);
		i = 1;
		std::cerr << "Notifying again...\n";
	}
	cv.notify_all();
}



static void* RenderThread(void* args) {
	 std::cout << "render thread create success.\n";
	 int frame_count = 0;
	 pthread_barrier_t* barriers = static_cast<pthread_barrier_t*>(args);
 
	 // render init
	 Window::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
 
	 std::cout << "render thread init success. waiting for logical thread init.\n";
	 // wait for logical init
	 pthread_barrier_wait(barriers + frame_count % 2);
 
	 while (!Input::GetKeyDown(GLFW_KEY_ESCAPE))
	 {
	 std::cout << "render thread: frame progress.\n";
	 RenderCommandList::Instance()->ConsumeRenderCommand(frame_count % 2);
	 ++frame_count;
	  std::cout << "render thread: frame completed. waiting for logical thread.\n";
	 pthread_barrier_wait(barriers + frame_count % 2);
	 Window::Instance()->Clear();
 }
 
 Engine::Instance()->Exit();
 // main thread and logical thread are still blocked. free them.
 pthread_barrier_wait( barriers + (frame_count + 1) % 2);
 
 Window::Instance()->Delete();
 std::cout << "render thread: exit.\n";
 return 0;
}
static void* LogicalThread(void* args) {
 std::cout << "logical thread create success.\n";
 int frame_count = 0;
 pthread_barrier_t* barriers = static_cast<pthread_barrier_t*>(args);
 
 // logical init
 GameTime::Instance();
 std::cout << "logical thread init success. waiting for render thread init.\n";
 // wait for render init
 pthread_barrier_wait(barriers + frame_count % 2);
 
 //auto call_init = [](MonoBehaviour* mono)->void {
 //mono->Init();
 //};
 //// mono init should be called after render init
 //MonoBehaviour::Traverse(call_init);
 //
 //auto call_update = [](MonoBehaviour* mono)->void {
 //mono->Update();
 //};
 
 while (!Engine::Instance()->ShouldExit())
 {
 std::cout << "logical thread: frame progress.\n";
 ++frame_count;
 RenderCommandList::Instance()->SetCurrentLogicalList(frame_count % 2);
 //Input::TestMouseDown();
 //MonoBehaviour::Traverse(call_update);
 //GameObject::RenderGameObject();
 std::cout << "logical thread: frame completed. waiting for render thread.\n";
 pthread_barrier_wait( barriers + frame_count % 2);
 }
 
 std::cout << "logical thread: exit.\n";
 return 0;
}

void Run(){

	 int frame_count = 0;
	 std::thread logical(LogicalThread);
	 std::thread render(RenderThread);
	 // 2 barriers to synchronize main thread, logical thread and render thread;
	 pthread_barrier_t barriers[2];
	 pthread_barrier_init(barriers, NULL, 3);
	 pthread_barrier_init(barriers + 1, NULL, 3);
 
	 int ret = pthread_create(&logical, NULL, LogicalThread, &barriers);
	 if (ret < 0) {
	 std::cout << "Failed to create logical thread." << std::endl;
	 return;
	 }
 
	 ret = pthread_create(&render, NULL, RenderThread, &barriers);
	 if (ret < 0) {
	 std::cout << "Failed to create render thread." << std::endl;
	 return;
	 }
 
	 pthread_barrier_wait(barriers + frame_count % 2);

 
	 std::cout << "main thread: thread init success. main while starts from here.\n";
	 while (!should_exit)
	 {

	 std::cout << "main thread: frame progress.\n";
	 ++frame_count;
	 pthread_barrier_destroy(barriers + (frame_count + 1) % 2);
	 pthread_barrier_init(barriers + (frame_count + 1) % 2, NULL, 3);
	 std::cout << "main thread: frame completed. waiting for child threads.\n";
	 pthread_barrier_wait( barriers + frame_count % 2);
 }
 

	 pthread_join(logical, NULL);
	 pthread_join(render, NULL);

}
int main(){

	Run();

	
}