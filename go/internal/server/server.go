package server

import (
	"fmt"
	"net/http"

	log "github.com/oryaacov/smart-home/pkg/log"

	"github.com/gin-gonic/gin"
	ginserver "github.com/go-oauth2/gin-server"
	"github.com/go-oauth2/oauth2/v4"
	"github.com/go-oauth2/oauth2/v4/server"
	"github.com/spf13/viper"
)

type Middlewares func(f http.HandlerFunc) http.HandlerFunc

type HTTPServer struct {
	logger               *log.Logger
	handler              *Handler
	authorizationManager oauth2.Manager
	server               *gin.Engine
}

func init() {
	viper.SetDefault(ListenningProtocolEnv, "http")
	viper.SetDefault(ListenningAddressEnv, "localhost")
	viper.SetDefault(ListenningPortEnv, "8000")
}

func Create() *HTTPServer {
	return &HTTPServer{
		logger:               log.Log(),
		authorizationManager: CreateOAuth2Manager(),
		handler:              CreateHandler(),
		server:               gin.Default(),
	}
}

func (s *HTTPServer) setRoutes() {
	smartRoom := s.server.Group("/smart-room")
	{
		auth := smartRoom.Group("/oauth2")
		{
			auth.GET("/token", ginserver.HandleTokenRequest)
		}
		api := smartRoom.Group("/api")
		{
			api.Use(ginserver.HandleTokenVerify())
			api.GET("/ac", s.handler.handleAC)
			api.GET("/test", s.handler.handleTokenTest)
		}
	}
}

func (s *HTTPServer) Listen() {
	ginserver.InitServer(s.authorizationManager)
	ginserver.SetAllowGetAccessRequest(true)
	ginserver.SetClientInfoHandler(server.ClientFormHandler)

	s.server = gin.Default()

	s.setRoutes()

	listeningAddress := viper.GetString(ListenningAddressEnv)
	listeningPort := viper.GetString(ListenningPortEnv)

	s.server.Run(fmt.Sprintf("%s:%s", listeningAddress, listeningPort))
}
