//https://zhuanlan.zhihu.com/p/339880005
#include <list>
#include <thread>
enum RenderCommandType {
 CreateMeshBuffer,
 SendToGPU,
 CompileShader,
 ShaderSetBool,
 ShaderSetInt,
 ShaderSetFloat,
 ShaderSetVec3,
 ShaderSetVec4,
 ShaderSetMat4,
 ShaderUse,
 ShaderDelete,
 TextureCreateTex2D,
 WindowDraw
};

struct RenderCommand {
 RenderCommandType command_type;
 void* args;
 RenderCommand(RenderCommandType command, void* _args) :
 command_type(command),
 args(_args) {
 }
};

class RenderCommandList {
public:
 static inline RenderCommandList* Instance() { return singleton; }
 // do NOT call this in logical thread!
 void ConsumeRenderCommand(int idx);
 void PushRenderCommand(RenderCommandType command, void* args);
 inline void SetCurrentLogicalList(int n) { current_logical_list = n; }
private:
 static RenderCommandList* singleton;
 
 std::list<RenderCommand>* render_command_lists[2];
 int current_logical_list = 0;
 // this variable is for glDrawElements
 int global_indices_count = 0;
 RenderCommandList();
};
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
 
 auto call_init = [](MonoBehaviour* mono)->void {
 mono->Init();
 };
 // mono init should be called after render init
 MonoBehaviour::Traverse(call_init);
 
 auto call_update = [](MonoBehaviour* mono)->void {
 mono->Update();
 };
 
 while (!Engine::Instance()->ShouldExit())
 {
 std::cout << "logical thread: frame progress.\n";
 ++frame_count;
 RenderCommandList::Instance()->SetCurrentLogicalList(frame_count % 2);
 Input::TestMouseDown();
 MonoBehaviour::Traverse(call_update);
 GameObject::RenderGameObject();
 std::cout << "logical thread: frame completed. waiting for render thread.\n";
 pthread_barrier_wait( barriers + frame_count % 2);
 }
 
 std::cout << "logical thread: exit.\n";
 return 0;
}

void Run(){

 int frame_count = 0;
 pthread_t logical, render;
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




}