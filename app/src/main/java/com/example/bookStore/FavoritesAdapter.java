package com.example.bookStore;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.curlssl.R;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

public class FavoritesAdapter extends RecyclerView.Adapter<FavoritesAdapter.ViewHolder> {

    private Context context;
    private List<String> books = new ArrayList<>();

    public FavoritesAdapter(List<String> books, Context context) {
        this.books = books;
        this.context = context;
    }

    public void addBook(String id, String title, String description, String thumbnailUrl) {
        // Adicione os elementos à lista na ordem correta
        String formattedId = removeDoubleAsterisks(id);
        String formattedTitle = removeDoubleAsterisks(title);
        String formattedDescription = removeDoubleAsterisks(description);
        String formattedThumbnailUrl = removeDoubleAsterisks(thumbnailUrl);

        Log.d("FavoritesAdapter", "XXXX addBook %s" + formattedId);
        Log.d("FavoritesAdapter", "XXXX addBook %s" + formattedTitle);
        Log.d("FavoritesAdapter", "XXXX addBook %s" + formattedDescription);
        Log.d("FavoritesAdapter", "XXXX addBook %s" + formattedThumbnailUrl);

        books.add(formattedId);
        books.add(formattedTitle);
        books.add(formattedDescription);
        books.add(formattedThumbnailUrl);

        // Notifique que um item foi inserido na última posição
        notifyItemInserted(books.size() / 4 );
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView textTitle;
        public TextView textDescription;
        public ImageView imageThumbnail;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            textTitle = itemView.findViewById(R.id.textTitle);
            textDescription = itemView.findViewById(R.id.textDescription);
            imageThumbnail = itemView.findViewById(R.id.imageThumbnail);
        }
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_book, parent, false);
        return new ViewHolder(view);
    }

    private String removeDoubleAsterisks(String input) {
        return input.replace("**", "");
    }


    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        // Certifique-se de que há dados suficientes para preencher um conjunto completo
        if (position * 4 < books.size()) {
            String item1 = books.get(position * 4);  // ID
            String item2 = books.get(position * 4 + 1);  // Título
            String item3 = books.get(position * 4 + 2);  // Descrição
            String item4 = books.get(position * 4 + 3);  // URL da Imagem


            Log.d("FavoritesAdapter", "XXXXXX-onBindViewHolder %s" + item2);
            Log.d("FavoritesAdapter", "XXXXXX-onBindViewHolde %s" + item3);
            Log.d("FavoritesAdapter", "XXXXXX-onBindViewHolde %s" + item4);


            holder.textTitle.setText(removeDoubleAsterisks(item2));
            holder.textDescription.setText(removeDoubleAsterisks(item3));
            Picasso.get().load(removeDoubleAsterisks(item4)).into(holder.imageThumbnail);

            holder.itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // Restante do código para tratar o clique do item
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return books.size() / 4;
    }
}
