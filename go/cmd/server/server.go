package main

import (
	server "github.com/oryaacov/smart-home/internal/server"
	log "github.com/oryaacov/smart-home/pkg/log"
	"github.com/spf13/viper"
)

const (
	serviceName = "smart-house"
)

var (
	logger = log.Initialize(serviceName)
)

func main() {
	viper.AutomaticEnv()
	server := server.Create()
	logger.Info("started")
	server.Listen()
}
