class Pile {
   public:
    double mDiameter, mSurfaceRoughness;

    Pile();
    Pile(double _Diameter);
    ~Pile();

    void setDiameter(double _D);
    void setSurfaceRoughness(double _r);

    bool isSlender(double _WaveLength);
};
