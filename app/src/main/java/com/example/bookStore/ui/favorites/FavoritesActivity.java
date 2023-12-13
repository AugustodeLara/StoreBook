package com.example.bookStore.ui.favorites;

import android.os.Bundle;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import android.util.Log;

import com.example.bookStore.ui.favorites.adapter.FavoritesAdapter;
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

        setupRecyclerView();

        loadAndDisplayFavoriteBooks();
    }

    private void setupRecyclerView() {
        recyclerView = findViewById(R.id.favoritesRecyclerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        adapter = new FavoritesAdapter(new ArrayList<>(), this);
        recyclerView.setAdapter(adapter);
    }

    private void loadAndDisplayFavoriteBooks() {
        String[] favoriteBooksArray = getFavoritesSaved();
        List<String> favoriteBooksList = Arrays.asList(favoriteBooksArray);

        for (String book : favoriteBooksList) {
            processBookString(book);
        }
    }

    private void processBookString(String bookString) {
        String[] bookDetails = bookString.split("\\*\\*");

        if (bookDetails.length >= 4) {
            String id = bookDetails[0];
            String title = bookDetails[1];
            String description = bookDetails[2];
            String image = bookDetails[3];

            Log.v("FavoritesActivity", String.format("Detalhes do livro: ID=%s, Title=%s, Description=%s, Image=%s", id, title, description, image));

            adapter.addBook(id, title, description, image);
        } else {
            Log.e("FavoritesActivity", "String de livro mal formatada: " + bookString);
        }
    }

    private native String[] getFavoritesSaved();

    static {
        System.loadLibrary("app");
    }
}
