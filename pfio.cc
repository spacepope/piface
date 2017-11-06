#include <node.h>
#include <v8.h>
#include <pifacedigital.h>

using namespace v8;

void PfioInit(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t hw_addr = 0;
	if (args.Length() >= 1) {
		hw_addr = Integer::New(isolate, args[0]->IntegerValue())->Value();
	}
	int pifacedigital_fd = pifacedigital_open(hw_addr);
	args.GetReturnValue().Set(pifacedigital_fd);
}

void PfioOpenNoInit(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t hw_addr = 0;
	if (args.Length() >= 1) {
		hw_addr = Integer::New(isolate, args[0]->IntegerValue())->Value();
	}
	int pifacedigital_fd = pifacedigital_open_noinit(hw_addr);
	args.GetReturnValue().Set(pifacedigital_fd);
}

void PfioDeinit(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t hw_addr = 0;
	if (args.Length() >= 1) {
		hw_addr = Integer::New(isolate, args[0]->IntegerValue())->Value();
	}
	pifacedigital_close(hw_addr);
}

void PfioDigitalRead(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t pin = Integer::New(isolate, args[0]->IntegerValue())->Value();
	uint8_t val = pifacedigital_digital_read(pin) ^ 1; // invert value to match original API
	args.GetReturnValue().Set(val);
}

void PfioDigitalWrite(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t pin = Integer::New(isolate, args[0]->IntegerValue())->Value();
	uint8_t val = Integer::New(isolate, args[1]->IntegerValue())->Value();
	pifacedigital_digital_write(pin, val);
}

void PfioReadInput(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t hw_addr = 0;
	if (args.Length() >= 1) {
		hw_addr = Integer::New(isolate, args[0]->IntegerValue())->Value();
	}
	uint8_t val = pifacedigital_read_reg(INPUT, hw_addr) ^ 0xFF; // invert value to match original API
	args.GetReturnValue().Set(val);
}

void PfioReadOutput(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t hw_addr = 0;
	if (args.Length() >= 1) {
		hw_addr = Integer::New(isolate, args[0]->IntegerValue())->Value();
	}
	uint8_t val = pifacedigital_read_reg(OUTPUT, hw_addr);
	args.GetReturnValue().Set(val);
}

void PfioWriteOutput(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	uint8_t val = Integer::New(isolate, args[0]->IntegerValue())->Value();
	uint8_t hw_addr = 0;
	if (args.Length() > 1) {
		hw_addr = Integer::New(isolate, args[1]->IntegerValue())->Value();
	}
	pifacedigital_write_reg(val, OUTPUT, hw_addr);
}

void PfioEnableInterrupts(const v8::FunctionCallbackInfo<v8::Value>& args) {
	int res = pifacedigital_enable_interrupts();
	bool ok = (res == 0);
	args.GetReturnValue().Set(ok);
}

void PfioDisableInterrupts(const v8::FunctionCallbackInfo<v8::Value>& args) {
	int res = pifacedigital_disable_interrupts();
	bool ok = (res == 0);
	args.GetReturnValue().Set(ok);
}

void PfioWaitForInput(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int timeout = Integer::New(isolate, args[0]->IntegerValue())->Value();
	uint8_t hw_addr = 0;
	if (args.Length() > 1) {
		hw_addr = Integer::New(isolate, args[1]->IntegerValue())->Value();
	}
	uint8_t inputs;
	int res = pifacedigital_wait_for_input(&inputs, timeout, hw_addr);
	if (res < 0) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Error waiting on interrupt."));
	} else if (res == 0) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Interrupt timed out."));
	} else {
		args.GetReturnValue().Set(inputs ^ 0xFF);
	}
}

void init(Handle<Object> exports) {
	NODE_SET_METHOD(exports, "init", PfioInit);
	NODE_SET_METHOD(exports, "open_noinit", PfioOpenNoInit);
	NODE_SET_METHOD(exports, "deinit", PfioDeinit);
	NODE_SET_METHOD(exports, "digital_read", PfioDigitalRead);
	NODE_SET_METHOD(exports, "digital_write", PfioDigitalWrite);
	NODE_SET_METHOD(exports, "read_input", PfioReadInput);
	NODE_SET_METHOD(exports, "read_output", PfioReadOutput);
	NODE_SET_METHOD(exports, "write_output", PfioWriteOutput);
	NODE_SET_METHOD(exports, "enable_interrupts", PfioEnableInterrupts);
	NODE_SET_METHOD(exports, "disable_interrupts", PfioDisableInterrupts);
	NODE_SET_METHOD(exports, "wait_for_input", PfioWaitForInput);
}

NODE_MODULE(pfio, init);
