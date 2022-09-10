package logger

import (
	"github.com/blendle/zapdriver"
	"github.com/spf13/viper"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

const (
	logLevelEnv = "LOG_LEVEL"
	traceLogLevel = "trace"
)

var client *Logger
var levelMap = map[string]zapcore.Level{
	"trace":    zapcore.DebugLevel,
	"debug":    zapcore.DebugLevel,
	"info":     zapcore.InfoLevel,
	"warn":     zapcore.WarnLevel,
	"error":    zapcore.ErrorLevel,
	"panic":    zapcore.PanicLevel,
	"fatal":    zapcore.FatalLevel,
	"devPanic": zapcore.DPanicLevel,
}

func init() {
	viper.SetDefault(logLevelEnv, "debug")
}

type Logger struct {
	logger       *zap.Logger
	traceEnabled bool
}

func getLevel(level string) zapcore.Level {
	if logLevel, ok := levelMap[level]; ok {
		return logLevel
	}

	return zapcore.DebugLevel
}

func Initialize(serviceName string) Logger {
	productionConfig := zapdriver.NewProductionConfig()
	logLevel := viper.GetString(logLevelEnv)
	productionConfig.Level = zap.NewAtomicLevelAt(getLevel(logLevel))
	 
	logger, err := productionConfig.Build(zapdriver.WrapCore(
		zapdriver.ReportAllErrors(true),
		zapdriver.ServiceName(serviceName)))

	if err != nil {
		panic(err)
	}

	return Logger{logger: logger, traceEnabled: logLevel == traceLogLevel}
}

func (c *Logger) Trace(msg string, fields ...zap.Field) {
	if !c.traceEnabled {
		return
	}

	fields = append(fields, zapdriver.Label("level", traceLogLevel))
	c.Debug(msg, fields...)
}

func (l *Logger) Debug(msg string, fields ...zap.Field) {
	l.logger.Debug(msg, fields...)
}

func (l *Logger) Info(msg string, fields ...zap.Field) {
	l.logger.Info(msg, fields...)
}

func (l *Logger) Warn(msg string, fields ...zap.Field) {
	l.logger.Warn(msg, fields...)
}

func (l *Logger) Error(msg string, fields ...zap.Field) {
	l.logger.Error(msg, fields...)
}

func (l *Logger) Panic(msg string, fields ...zap.Field) {
	l.logger.Panic(msg, fields...)
}

func Log() *Logger {
	return client
}