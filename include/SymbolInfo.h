#ifndef SYMBOLINFO_H
#define SYMBOLINFO_H


class SymbolInfo
{
    public:
        SymbolInfo(int locInMemory, char type);
        int getLocation();
        char getType();

    protected:

    private:
        int location;
        char symbolType;
};

#endif // SYMBOLINFO_H
