
No Lua, faz-se:

    myproxyclass = proxy "CXX_Class"
    -- then add attributes and methods.
    -- overloading virtual methods is possible

No SWIG-proxy do Lua:

    Fazer função "proxy".
    Registrar classes proxiadas.
    A função "proxy" faz:
        + Cria um userdata a partir de um ClassProxy
        + Esse objeto ClassProxy recebe uma tabela como vobj
        + Dá ao userdata a seguinte metatabela:
            __index --> vobj[k] or super[k]
            __newindex --> vobj[k] = v