# Ninja

## 1. Hướng dẫn cài đặt:

Sau khi cài đặt [MingGW](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-win32/seh/x86_64-7.3.0-release-win32-seh-rt_v5-rev0.7z/download), [SDL2](https://www.libsdl.org/release/SDL2-devel-2.0.20-mingw.tar.gz), [SDL_Image](https://www.libsdl.org/projects/SDL_image/), [SDL_TTF](https://www.libsdl.org/projects/SDL_ttf/) và [SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/) thực hiện theo các bước sau:
- Mở PowerShell
- Clone repository bằng lệnh:  ``git clone https://github.com/bvhieu/Ninja.git``
- Mở folder vừa download bằng lệnh: ``cd .\Ninja\Ninja\``
- Biên dịch chương trình bằng lệnh: ``mingw32-make.exe``
- Sau đó mở App bằng lệnh: ``.\Ninja.exe``
    
## 2. Mô tả chung về trò chơi, các chức năng, các ý tưởng chính
- Trò chơi lấy ý tưởng từ nhân vật Ninja Nhật Bản
- Cách chơi: Người chơi sẽ di chuyển nhân vật Ninja trên bản đồ bằng bàn phím và tấn công các con cóc xanh trên bản đồ bằng cách nhảy lên đầu hoặc bắn ra các phi tiêu(phím SHIFT) để  để tiêu diệt cóc xanh đồng thời cần di chuyển để tránh rơi xuống vực và tránh các chùm lửa của rồng lửa(rồng lửa bất tử nên không thể tiêu diệt), tiêu diệt mỗi con cóc xanh bằng phi tiêu được `+200 điểm`, tiêu diệt cóc xanh bằng cách nhảy lên đầu cóc xanh thì được `+100 điểm`(tuy nhiên khi nhảy lên đầu cóc xanh không chết mà chỉ đứng yên nên khi chạm vào xác cóc xanh ta vẫn bị tấn công, khi này để tiêu diệt hoàn toàn cóc xanh ta phải bắn phi tiêu lên cái xác đó và được thêm `+200 điểm`),  thời gian của bản đồ sẽ là `300 giây` đếm ngược, mỗi người chơi sẽ có `3` sinh mệnh, khi hết thời gian hoặc bị con cóc xanh tấn công thì sẽ mất `1` sinh mệnh cho đến khi hết sinh mệnh thì màn hình sẽ hiện `GAME OVER`, điểm cao nhất sẽ luôn hiển thị trên màn hình và chỉ reset lại về `0 điểm` khi đóng chương trình.
- Các chức năng: 
    + Tính điểm
    + Lưu điểm cao nhất
    + Tính thời gian
    + Tuỳ chỉnh âm lượng
    + Tuỳ chỉnh phím điểu khiển 

## 3. Link video Youtube
https://youtu.be/QATuNRV5ZsY

## 4. Các kĩ thuật sử dụng
- Sử dụng các thư viện SDL
- Tạo animation và FPS
- Tạo và quản lí các đối tượng: nhân vật, quái, bản đồ,...
- Tạo map, menu,... của trò chơi
- Mảng, con trỏ, cấu trúc, lớp, đồ họa ...

## 5. Kết luận:
Hiểu được cách cài đặt các môi trường, thư viện, cách tạo ra một chương trình và cách vận hành, quản lí chương trình.

### Hướng phát triển:
    - Tạo thêm các level
    - Thêm chức năng tăng chỉ số sinh mệnh khi lấy được trái tim
    - Thêm chức năng nhiều người cùng chơi
    - ...

### Các điều tâm đắc rút ra:
    - Tách các file ra và để mỗi file quản lí một đối tượng khác nhau của chương trình giúp dễ dàng quản lí và chỉnh sửa
## Nguồn tham khảo:
- Tham khảo hình ảnh trong game Ninja School
- https://lazyfoo.net/tutorials/SDL/
- https://phattrienphanmem123az.com/
- https://mixkit.co/
