package server

import (
	"fmt"

	"github.com/go-oauth2/oauth2/v4/manage"
	"github.com/go-oauth2/oauth2/v4/models"
	"github.com/go-oauth2/oauth2/v4/store"
	"github.com/spf13/viper"
)

func init() {
	viper.SetDefault(OAuthClientIDEnv, "000000")
	viper.SetDefault(OAuthSecretEnv, "999999")
	viper.SetDefault(DomainEnv, "http://localhost")
}

func CreateOAuth2Manager() *manage.Manager {
	manager := manage.NewDefaultManager()
	manager.MustTokenStorage(store.NewMemoryTokenStore())

	clientID := viper.GetString(OAuthClientIDEnv)
	clientSecret := viper.GetString(OAuthSecretEnv)
	domain := viper.GetString(DomainEnv)

	fmt.Println("test", clientID, clientSecret, domain)

	clientStore := store.NewClientStore()
	clientStore.Set(clientID, &models.Client{
		ID:     clientID,
		Secret: clientSecret,
		Domain: domain,
	})

	manager.MapClientStorage(clientStore)

	return manager
}
