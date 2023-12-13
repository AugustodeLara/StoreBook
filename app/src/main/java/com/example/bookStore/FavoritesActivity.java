package com.example.bookStore;

import android.os.Bundle;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import android.util.Log;

import com.example.curlssl.R;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class FavoritesActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private FavoritesAdapter adapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_favorites);

        recyclerView = findViewById(R.id.favoritesRecyclerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        adapter = new FavoritesAdapter(new ArrayList<>(), this);

        loadAndDisplayFavoriteBooks();
    }

    private void loadAndDisplayFavoriteBooks() {
        // Chamar a função JNI para obter os livros favoritos do CSV
        String[] favoriteBooksArray = getFavoritesSaved();

        // Converter o array para uma lista para facilitar o uso
        List<String> favoriteBooksList = Arrays.asList(favoriteBooksArray);

        // Processar cada string de livro e adicionar à lista do adaptador
        for (String book : favoriteBooksList) {
            processBookString(book);
        }

        // Definir o adaptador na RecyclerView
        recyclerView.setAdapter(adapter);
    }

    private void processBookString(String bookString) {
        // Remover delimitadores duplicados e garantir que haja pelo menos 4 partes
        String[] bookDetails = bookString.split("\\*\\*");


        // Verificar se há pelo menos 4 partes (ID, Título, Descrição, URL da Imagem)
        if (bookDetails.length >= 4) {
            String id = bookDetails[0];
            String title = bookDetails[1];
            String description = bookDetails[2];
            String image = bookDetails[3];

            Log.v("FavoritesActivity", "Detalhes do livro: ID=" + id + ", Title=" + title + ", Description=" + description + ", Image=" + image);

            // Adicionar à lista do adaptador
            adapter.addBook(id, title, description, image);
        } else {
            Log.e("FavoritesActivity", "String de livro mal formatada: " + bookString);
        }
    }

    // Declaração da função JNI para obter os livros favoritos
    private native String[] getFavoritesSaved();

    static {
        System.loadLibrary("app");
    }
}
