package com.example.bookStore;

import android.content.Intent;
import android.content.res.ColorStateList;
import android.net.Uri;
import android.util.Log;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.curlssl.R;
import com.squareup.picasso.Picasso;

public class BookDetailActivity extends AppCompatActivity {

    private boolean isBookFavorited = false; // Adicione esta variável de estado

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_book_detail);

        // Adicione esta linha para configurar a seta de volta na barra de ação
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        String id = getIntent().getStringExtra("id");
        String title = getIntent().getStringExtra("title");
        String description = getIntent().getStringExtra("description");
        String thumbnailUrl = getIntent().getStringExtra("thumbnailUrl");
        boolean isFavorite = isFavorite(id);  // Assumindo que isFavorite retorna um booleano
        String favorite = String.valueOf(isFavorite);
        String buyLink = getIntent().getStringExtra("buyLink");

        Log.d("BookDetailActivity", "buyLink: " + buyLink);
        Log.d("BookDetailActivity", "favorite: " + favorite);


        // Configure os elementos da interface do usuário com os dados do livro
        TextView idTextView = findViewById(R.id.detailId);
        TextView titleTextView = findViewById(R.id.detailTitle);
        TextView descriptionTextView = findViewById(R.id.detailDescription);
        ImageView thumbnailImageView = findViewById(R.id.detailThumbnail);

        idTextView.setText("ID: " + id);  // Adicione o ID aqui
        titleTextView.setText(title);
        descriptionTextView.setText(description);
        Picasso.get().load(thumbnailUrl).into(thumbnailImageView);

        // Configurar o clique no botão de Favoritar
        Button favoriteButton = findViewById(R.id.buttonFavorite);

        Toast.makeText(BookDetailActivity.this, "favoriteButton: " + favorite, Toast.LENGTH_SHORT).show();


        //TODO: Ajustar favoritos

        // Verificar o valor de "favorite" e configurar a cor do botão
        if (favorite == "true") {
            isBookFavorited = true;
            favoriteButton.setText("FAVORITO");
            favoriteButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.holo_green_light)));
        } else {
            isBookFavorited = false;
            favoriteButton.setText("FAVORITAR");

            favoriteButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.holo_red_light)));
        }

        favoriteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // Verificar se o livro já foi favoritado
                if (!isBookFavorited) {
                    // Chamar a função C++ para favoritar o livro e salvar no arquivo CSV
                    favoriteBook(id, title, description, thumbnailUrl);

                    // Implemente aqui a lógica para favoritar o livro
                    Toast.makeText(BookDetailActivity.this, "Livro Favoritado!", Toast.LENGTH_SHORT).show();

                    // Atualizar o estado e a cor do botão
                    isBookFavorited = true;
                    favoriteButton.setText("FAVORITO");

                    favoriteButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.holo_green_light)));

                } else {
                    // Implemente aqui a lógica para desfavoritar o livro
                    Toast.makeText(BookDetailActivity.this, "Livro Desfavoritado!", Toast.LENGTH_SHORT).show();
                    unfollowBook(id);
                    // Atualizar o estado e a cor do botão
                    favoriteButton.setText("FAVORITAR");

                    isBookFavorited = false;
                    favoriteButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.holo_red_light)));
                }
            }
        });

        if("Indisponivel".equals(buyLink.trim())){
            Log.d("BookDetailActivity", "xxxxxxxxxx - IGUAL: " + buyLink);
        }

        Button buyButton = findViewById(R.id.buttonBuy);
        if ("Indisponivel".equals(buyLink.trim())) {
            // Se o BuyLink estiver indisponível, tornar o botão cinza e indisponível
            buyButton.setVisibility(View.VISIBLE);
            buyButton.setEnabled(false);  // Desabilitar o botão
            buyButton.setBackgroundTintList(ColorStateList.valueOf(getResources().getColor(android.R.color.darker_gray)));
            buyButton.setBackgroundColor(getResources().getColor(android.R.color.darker_gray));
            buyButton.setText("Indisponível");
        } else if (buyLink != null && !buyLink.isEmpty()) {
            // Se o BuyLink estiver disponível, configurar o botão normalmente
            buyButton.setVisibility(View.VISIBLE);
            buyButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // Abrir o link de compra no navegador
                    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(buyLink));
                    // Verificar se há um aplicativo para manipular a intenção
                    if (browserIntent.resolveActivity(getPackageManager()) != null) {
                        startActivity(browserIntent);
                    } else {
                        // Caso não haja aplicativo para manipular a intenção, mostrar mensagem
                        Toast.makeText(BookDetailActivity.this, "Nenhum navegador disponível", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        } else {
            // Se o BuyLink não estiver disponível, ocultar o botão
            buyButton.setVisibility(View.GONE);
        }
    }

    // Adicione este método JNI para chamar a função C++ de favoritar o livro
    private native void favoriteBook(String id, String title, String description, String thumbnail);

    private native boolean unfollowBook(String id);

    private native boolean isFavorite(String id);

    // Carregue a biblioteca nativa
    static {
        System.loadLibrary("app");
    }
}
