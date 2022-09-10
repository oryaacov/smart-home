package server

import (
	"net/http"

	"github.com/gin-gonic/gin"
	ginserver "github.com/go-oauth2/gin-server"
)

type Handler struct {
}

func CreateHandler() *Handler {
	return &Handler{}
}

func (h *Handler) handleAC(c *gin.Context) {
	c.String(http.StatusOK, "AC!")
}

func (h *Handler) handleTokenTest(c *gin.Context) {
	ti, exists := c.Get(ginserver.DefaultConfig.TokenKey)
	if exists {
		c.JSON(http.StatusOK, ti)
		return
	}

	c.String(http.StatusOK, "not found")
}
