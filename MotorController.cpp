#include <iostream>
using namespace std;
struct motor
{
    int speed;
    int direction;
};
void ShowMenu()
{
    cout<<"请输入数字来选择你的模式："<< endl;
    cout<<"0.关闭电机"<< endl;
    cout<<"1.开启电机"<< endl;
    cout<<"2.修改电机速度"<< endl;
    cout<<"3.查看当前电机速度"<< endl;
    cout<<"4.改变电机旋转方向"<< endl;
    cout<<"5.查看当前电机旋转方向"<< endl;
}
void modifyspeed(motor * p)
{
    int m_speed=0;
    cout<<"请输入电机速度"<< endl;
    cin >> m_speed;
    p->speed=m_speed;
}
void changedir(motor * q)
{
    cout<<"请输入电机的旋转方向"<< endl;
    cout<<"1.顺时针方向"<< endl;
    cout<<"2.逆时针方向"<< endl;
    int m_direction;
    cin >> m_direction;
    q->direction=m_direction;
}
int main()
{
    motor motorbye;
    motorbye.speed=0;
    int select =0;
    ShowMenu();
    while (true)
    {
        cin >> select;
        if (select==1)
        {
            cout<<"电机已打开"<<endl;
            while (true)
            {
                ShowMenu();
                cin>> select;
                switch(select)
                {
                    case 0:
                    {
                        cout<<"电机已关闭"<< endl;
                        return 0;
                        break;
                    }
                    case 1:
                    {
                        cout<<"无效操作"<< endl;
                        break;
                    }
                    case 2:
                    {
                        modifyspeed(&motorbye);
                        break;
                    }
                    case 3:
                    {
                        cout << motorbye.speed << endl;
                        break;
                    }
                    case 4 :
                    {
                        changedir(&motorbye);
                        break;
                    }
                    case 5:
                    {
                        cout<<(motorbye.direction==1 ? "顺时针":"逆时针")<< endl;
                        break;
                    }

                }
            
            }
            return 0;
        }
        else
        {
            cout<<"请先打开电机"<< endl;
            continue;

        }
    
    }
}