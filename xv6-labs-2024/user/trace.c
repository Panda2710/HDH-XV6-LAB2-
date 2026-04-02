#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  // Kiểm tra xem người dùng có nhập đủ cú pháp không
  // Cú pháp đúng: trace <mask> <command> [args...]
  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  
  
  // Gọi system call trace()  trong kernel.
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }

  // Chuẩn bị mảng đối số  cho lệnh cần chạy tiếp theo
  // Bỏ qua argv[0] ("trace") và argv[1] ("mask")
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[i-2] = 0; // Đánh dấu kết thúc mảng bằng NULL

  
  // Lệnh exec() sẽ thay thế chương trình trace hiện tại bằng chương trình mới

  exec(nargv[0], nargv);

  // lệnh lỗi nếu exec() trả về và in ra lỗi
  printf("exec %s failed\n", nargv[0]);
  exit(1);
}