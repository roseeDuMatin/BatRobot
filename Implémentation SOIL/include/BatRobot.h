#ifndef BATROBOT_H
#define BATROBOT_H


class BatRobot
{
    public:
        BatRobot();
        BatRobot(float x, float y, float z);
        void Draw();
       // void SetTexture(int face, GLuint texture);

        float posx;
        float posy;
        float posz;

        float x;
        float y;
        float z;

    protected:

    private:
        //GLuint textures[6];
        char anchor[3];
};

#endif // BATROBOT_H
