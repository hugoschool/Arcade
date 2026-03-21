%.so:	CXXFLAGS	+=	-fPIC
%.so:
	mkdir -p $(dir $@)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)
