
default: build

%.json: %.fbp
	./node_modules/.bin/fbp $< > $@

env:
	mkdir bin || echo 'Exists'

bin/simulator: env graphs/main.json
	g++ -Werror -Wall -std=c++11 -g ./tools/simulator.cpp -o ./bin/simulator -DHAVE_JSON11 -I./thirdparty/json11

build: bin/simulator

clean:
	git clean -dfx --exclude node_modules
