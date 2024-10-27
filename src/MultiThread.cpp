//https://zhuanlan.zhihu.com/p/339880005
//https://www.cnblogs.com/yang131/p/16753516.html
//https://winddoing.github.io/post/44245d70.html
// //egl
//https://keyou.github.io/blog/2020/06/12/opengl-sync/
#include <list>
#include <thread>
#include <condition_variable>
#include "RenderCommandList.h"
#include "Window.h"
#include "Engine.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


class Semaphore {
public:
	Semaphore() {

	}
	void Wait() {
		std::unique_lock<std::mutex> lock(m);
		v.wait(lock, [&] { return signal == true; });
		signal = false;
	 }
	void Signal(){
		std::unique_lock<std::mutex> lock(m);
		signal = true;
		v.notify_one();
	}
	
	std::atomic<bool> signal;
	std::mutex m;
	std::condition_variable v;
};
Semaphore logic_barriers;
Semaphore render_barriers;
//std::atomic<bool> signal;
//std::mutex m;
bool should_exit = false;
std::mutex framte_mutex;
//
//void waits(std::condition_variable& v)
//{
//	std::unique_lock<std::mutex> lock(m);
//	v.wait(lock, [&] { return signal == true; });
//}
//
//void signals(std::condition_variable& v)
//{
//	std::unique_lock<std::mutex> lock(m);
//	signal = true;
//	v.notify_one();
//}



static void* RenderThread() {//Semaphore& logic_barriers, Semaphore& render_barriers
	std::cout << "render thread create success.\n";
	int frame_count = 0;
	//std::condition_variable* barriers = static_cast<std::condition_variable*>(args);

	// render init
	Window::Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	std::cout << "render thread init success. waiting for logical thread init.\n";


	while (!Window::Instance()->ShouldExit())
	{
		logic_barriers.Wait();

		framte_mutex.lock();
		std::cout << "[render thread] frame progress id =" << frame_count <<".\n";
		RenderCommandList::Instance()->ConsumeRenderCommand(frame_count % 2);
		framte_mutex.unlock();

		render_barriers.Signal();
		++frame_count;
		//std::cout << "render thread: frame completed. waiting for logical thread.\n";
		Window::Instance()->Clear();

	}

	Engine::Instance()->Exit();

	Window::Instance()->Delete();
	std::cout << "render thread: exit.\n";
	return 0;
}

void Run() {

	int frame_count = 0;
	//std::condition_variable logic_barriers;
	//std::condition_variable render_barriers;
	//std::condition_variable barriers[2];


	std::thread render(RenderThread);

	render_barriers.Signal();

	//push 0 frame
	std::cout << "main thread: thread init success. main while starts from here.\n";
	while (!should_exit)
	{
		render_barriers.Wait();
		framte_mutex.lock();
		std::cout << "[main thread] frame progress id =" << frame_count << ".\n";

		RenderCommandList::Instance()->PushRenderCommand(frame_count % 2, CompileShader, nullptr);
		framte_mutex.unlock();

		logic_barriers.Signal();

		++frame_count;


		//std::cout << "main thread: frame completed. waiting for child threads.\n";

	}

	//logical.join();
	render.join();
	/* pthread_join(logical, NULL);
	 pthread_join(render, NULL);*/

}
int main() {

	Run();


}


//static void* LogicalThread() {
//	std::cout << "logical thread create success.\n";
//	int frame_count = 0;
//	//std::condition_variable* barriers = static_cast<std::condition_variable*>(args);
//
//	// logical init
//	//GameTime::Instance();
//	std::cout << "logical thread init success. waiting for render thread init.\n";
//	// wait for render init
//	//waits(barriers[frame_count % 2]);
//
//	//auto call_init = [](MonoBehaviour* mono)->void {
//	//mono->Init();
//	//};
//	//// mono init should be called after render init
//	//MonoBehaviour::Traverse(call_init);
//	//
//	//auto call_update = [](MonoBehaviour* mono)->void {
//	//mono->Update();
//	//};
//
//	while (!Window::Instance()->ShouldExit())
//	{
//		std::cout << "logical thread: frame progress.\n";
//
//		framte_mutex.lock();
//
//		RenderCommandList::Instance()->SetCurrentLogicalList(frame_count % 2);
//		//Input::TestMouseDown();
//		//MonoBehaviour::Traverse(call_update);
//		//GameObject::RenderGameObject();
//		std::cout << "logical thread: frame completed. waiting for render thread.\n";
//		framte_mutex.unlock();
//		//signals(barriers[frame_count % 2]);
//		++frame_count;
//
//
//	}
//
//	std::cout << "logical thread: exit.\n";
//	return 0;
//}
